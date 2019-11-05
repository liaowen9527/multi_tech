#include "stdafx.h"
#include "NBLog.h"
#include "tcp_server.h"
#include <functional>
#include <iostream>
#include "boost/asio.hpp"
#include <mutex>
#include "boost/asio/ip/address.hpp"
#include <thread>

#pragma  comment(lib, "ssleay32.lib")
#pragma  comment(lib, "libeay32.lib")

namespace netkit {

	std::string TcpConnection::m_headDelim = PacketHead::GetDelimeter();

	TcpConnection::TcpConnection(TcpSockPtr socket, bool bUseInternalUnzip)
		: m_tcpSock(std::move(socket))
		, m_connected(true)
		, m_lastReqTime(0)
		, m_trustful(false)
		, m_strand(m_tcpSock->get_io_service())
		, m_bUseInternalUnzip(bUseInternalUnzip)
	{
	}

	TcpConnection::TcpConnection(SslSockPtr socket, bool bUseInternalUnzip)
		: m_sslSock(std::move(socket))
		, m_connected(true)
		, m_lastReqTime(0)
		, m_trustful(false)
		, m_strand(m_sslSock->get_io_service())
		, m_bUseInternalUnzip(bUseInternalUnzip)
	{
	}

	TcpConnection::~TcpConnection()
	{
		NetkitLog(InstNetkit, LogLevelInfo, "this = %p", this);
	}

	void TcpConnection::SetHandler(PackHandlerPtr handler)
	{
		m_packHandler = handler;
	}

	bool TcpConnection::Connected()
	{
		return m_connected;
	}

	void TcpConnection::Read()
	{
		std::lock_guard<SocketLock> lock(m_sockLock);
		auto func = std::bind(&TcpConnection::OnReadHead, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
		m_tcpSock.get() ? async_read_until(*m_tcpSock, m_buffer, m_headDelim, m_strand.wrap(func)) : async_read_until(*m_sslSock, m_buffer, m_headDelim, m_strand.wrap(func));
	}

	bool TcpConnection::ExtractHeadFromBuffer(std::string &head)
	{
		std::lock_guard<SocketLock> lock(m_sockLock);

		if (m_buffer.size() == 0)
		{
			return false;
		}

		auto buf = boost::asio::buffer_cast<const char*>(m_buffer.data());
		auto headDelim = PacketHead::FindDelimeter(buf, m_buffer.size());
		if (!headDelim)
		{
			return false;
		}

		size_t headLen = headDelim - buf;
		head.append(buf, headLen);
		m_buffer.consume(headLen + m_headDelim.size());

		return true;
	}

	bool TcpConnection::ExtractBodyFromBuffer(PacketPtr &pack, size_t &extractedSize)
	{
		std::lock_guard<SocketLock> lock(m_sockLock);

		auto buf = boost::asio::buffer_cast<const char*>(m_buffer.data());
		auto bufSize = m_buffer.size();
		auto bodySize = pack->head.GetBodySize();
		
		auto extrSize = std::min<size_t>(bufSize, bodySize);
		memcpy((char *)pack->body->data(), buf, extrSize);
		m_buffer.consume(extrSize);
		extractedSize = extrSize;

		if (bufSize < bodySize)
		{
			return false;
		}
		
		return true;
	}

	void TcpConnection::OnReadHead(ErrorCode ec, std::size_t byteCnt)
	{
		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "read head error, %d %s, conn id %s", (int)ec.value(), ec.message().c_str(), m_id.c_str());
			Close();
			return;
		}

		while (true)
		{
			std::string headData;
			if (!ExtractHeadFromBuffer(headData))
			{
				Read();
				break;
			}

			NetkitLog(InstNetkit, LogLevelDebug, "recv packet head: %s", headData.c_str());

			PacketPtr pack(new Packet(false));
			if (!pack->head.FromString(headData))
			{
				NetkitLog(InstNetkit, LogLevelError, "Invalid protocol head, conn id %s", m_id.c_str());
				Close();
				return;
			}

			if (!pack->head.CheckVersion())
			{
				NetkitLog(InstNetkit, LogLevelError, "Invalid protocol version, conn id %s", m_id.c_str());
				Close();
				return;
			}

			if (!pack->AllocBody())
			{
				NetkitLog(InstNetkit, LogLevelError, "Alloc body fail, conn id %s", m_id.c_str());
				Close();
				return;
			}
			
			size_t extractedSize = 0;
			if (!ExtractBodyFromBuffer(pack, extractedSize))
			{
				std::lock_guard<SocketLock> lock(m_sockLock);
				
				size_t readSize = extractedSize;
				size_t unReadSize = pack->head.GetBodySize() - readSize;
				auto buf = boost::asio::buffer((char*)pack->body->data() + readSize, unReadSize);
				auto func = std::bind(&TcpConnection::OnReadBody, shared_from_this(), std::placeholders::_1, std::placeholders::_2, pack, readSize);
				m_tcpSock.get() ? async_read(*m_tcpSock, buf, m_strand.wrap(func)) : async_read(*m_sslSock, buf, m_strand.wrap(func));

				break;
			}

			OnRead(pack);
		}
	}

	void TcpConnection::OnReadBody(ErrorCode ec, std::size_t byteCnt, PacketPtr pack, size_t bodyDataSize)
	{
		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "read body error, %s, conn id %s", ec.message().c_str(), m_id.c_str());
			Close();
			return;
		}

		auto bodySize = pack->head.GetBodySize();
		auto readSize = bodyDataSize + byteCnt;
		if (readSize != bodySize)
		{
			NetkitLog(InstNetkit, LogLevelError, "read body size %d != head body size %d, conn id %s", readSize, bodySize, m_id.c_str());
			assert(false);
			Close();
			return;
		}

		//pack->body->_Get_data()._Mysize = bodySize;
		if (pack->body->capacity() > bodySize)
		{
			*(char*)(pack->body->data() + bodySize) = '\0';
		}

		OnRead(pack);
		Read();
	}

	void TcpConnection::OnRead(PacketPtr pack)
	{
		NetkitLog(InstNetkit, LogLevelDebug, "recv packet, body size %d", pack->body->size());

		if (m_bUseInternalUnzip && !PackHelper::Unpack(pack))
		{
			NetkitLog(InstNetkit, LogLevelError, "unpack fail, conn id %s", m_id.c_str());
			return;
		}

		m_packHandler->OnRead(shared_from_this(), pack);
	}

	void TcpConnection::Write(const BinaryDataPtr &data, WriteCallback callback)
	{
		assert(callback);

		std::lock_guard<SocketLock> lock(m_sockLock);
		if (m_writeQueue.empty())
		{
			m_strand.post(std::bind(&TcpConnection::WriteDirectly, shared_from_this(), data, callback));
		}
		
		m_writeQueue.push(WriteInfo{ data, callback });
	}

	void TcpConnection::WriteDirectly(const BinaryDataPtr &data, WriteCallback callback)
	{
		auto buf = boost::asio::buffer(data->data(), data->size());
		auto func = std::bind(&TcpConnection::OnWriteData, shared_from_this(), std::placeholders::_1, std::placeholders::_2, callback, data);
		std::lock_guard<SocketLock> lock(m_sockLock);
		m_tcpSock.get() ? async_write(*m_tcpSock, buf, m_strand.wrap(func)) : async_write(*m_sslSock, buf, m_strand.wrap(func));
	}

	void TcpConnection::OnWriteData(ErrorCode ec, std::size_t byteCnt, WriteCallback callback, BinaryDataPtr data)
	{
		data.reset();

		if (callback)
		{
			WriteResultPtr ret(new WriteResult);
			ret->conn = shared_from_this();
			ret->ec = ec;
			m_packHandler->OnWrite(ret, callback);
		}

		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "write packet error, %s, conn id %s", ec.message().c_str(), m_id.c_str());
			Close();
			return;
		}

		std::lock_guard<SocketLock> lock(m_sockLock);
		if (!m_writeQueue.empty())
		{
			m_writeQueue.pop();
		}

		if (!m_writeQueue.empty())
		{
			auto writeInfo = m_writeQueue.front();
			m_strand.post(std::bind(&TcpConnection::WriteDirectly, shared_from_this(), writeInfo.data, writeInfo.callback));
		}
		else
		{
			std::queue<WriteInfo>().swap(m_writeQueue);
		}
	}

	void TcpConnection::Close()
	{
		NetkitLog(InstNetkit, LogLevelWarn, "close conn, connected %d, conn id %s", (int)m_connected, m_id.c_str());

		{
			std::lock_guard<SocketLock> lock(m_sockLock);
			ErrorCode ec;
			if (m_tcpSock.get())
			{
				m_tcpSock->shutdown(tcp_socket::shutdown_both, ec);
				m_tcpSock->close(ec);
			}
			else if (m_sslSock.get())
			{
				//m_sslSock->shutdown(ec);
				m_sslSock->lowest_layer().close(ec);
			}

			if (ec)
			{
				NetkitLog(InstNetkit, LogLevelError, "close socket errror, %s", ec.message().c_str());
			}
		}

		if (m_connected)
		{
			OnClose();
		}
	}

	void TcpConnection::SetId(const std::string &id)
	{
		m_id = id;
	}

	std::string TcpConnection::GetId()
	{
		return m_id;
	}

	std::string TcpConnection::GetLocalAddress()
	{
		try
		{
			if (m_tcpSock.get())
			{
				return m_tcpSock->local_endpoint().address().to_string();
			}
			else if (m_sslSock.get())
			{
				return m_sslSock->lowest_layer().local_endpoint().address().to_string();
			}
		}
		catch (const std::exception &e)
		{
			NetkitLog(InstNetkit, LogLevelError, "get local address fail, %s", e.what());
		}

		return "";
	}

	time_t TcpConnection::GetLastRequestTime()
	{
		return m_lastReqTime;
	}

	void TcpConnection::SetLastRequestTime(time_t tim)
	{
		m_lastReqTime = tim;
	}

	bool TcpConnection::GetTrustful()
	{
		return m_trustful;
	}

	void TcpConnection::SetTrustful(bool trustful)
	{
		NetkitLog(InstNetkit, LogLevelWarn, "set conn trustful to %s, conn id %s", trustful ? "true" : "false", m_id.c_str());
		m_trustful = trustful;
	}

	void TcpConnection::OnClose()
	{
		m_connected = false;
		m_packHandler->OnClose(shared_from_this());
	}

	void TcpConnection::OnError(const ErrorType err, const ErrorCode &ec)
	{
		m_packHandler->OnError(shared_from_this(), err, ec);
		Close();
	}

}

