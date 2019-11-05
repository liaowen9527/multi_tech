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

	//////////////////////////////////////////////////////////////////////

	TcpServer::TcpServer(const ServerConfig &config)
		: m_ios(config.ioThdCnt)
		, m_wks(config.wkThdCnt)
		, m_acceptor(m_ios)
		, m_config(config)
	{
		NetkitLog(InstNetkit, LogLevelInfo, "use ssl %s", m_config.ssl ? "true" : "false");

		if (m_config.ssl)
		{
			m_sslCtx.reset(new boost::asio::ssl::context(boost::asio::ssl::context::sslv23_server));
		}
	}

	bool TcpServer::Start()
	{
		NetkitLog(InstNetkit, LogLevelInfo, "netkit server start");

		if (m_config.ssl)
		{
			try
			{
				m_sslCtx->set_options(boost::asio::ssl::context::default_workarounds);
				m_sslCtx->use_certificate_chain_file(m_config.sslCertFile);
				m_sslCtx->use_private_key_file(m_config.sslKeyFile, boost::asio::ssl::context::pem);
			}
			catch (const std::exception &e)
			{
				NetkitLog(InstNetkit, LogLevelError, "set ssl option fail, %s", e.what());
				return false;
			}
		}

		auto address = boost::asio::ip::address::from_string(m_config.addr);
		boost::asio::ip::tcp::endpoint endpoint{ address, m_config.port };
		ErrorCode ec;

		// Open the acceptor
		m_acceptor.open(endpoint.protocol(), ec);
		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "acceptor open %s:%d fail, %s", m_config.addr.c_str(), m_config.port, ec.message().c_str());
			return false;
		}

		if (m_config.reuseAddr)
		{
			m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
			if (ec)
			{
				NetkitLog(InstNetkit, LogLevelError, "acceptor set reuse addr option fail, %s", ec.message().c_str());
			}

#ifdef NB_LINUX
			int enabled = 1;
			setsockopt(m_acceptor.native_handle(), SOL_SOCKET, SO_REUSEPORT, (const char *)&enabled, sizeof(enabled));
#endif
		}

		// Bind to the server address
		m_acceptor.bind(endpoint, ec);
		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "acceptor bind to %s:%d fail, %s", m_config.addr.c_str(), m_config.port, ec.message().c_str());
			return false;
		}

		// Start listening for connections
		m_acceptor.listen(boost::asio::socket_base::max_connections, ec);
		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "acceptor listen fail, %s", ec.message().c_str());
			return false;
		}

		Run();

		return true;
	}

	void TcpServer::Run()
	{
		if (!m_acceptor.is_open())
		{
			NetkitLog(InstNetkit, LogLevelError, "acceptor not opened");
			return;
		}

		Accept();

		for (auto i = 0; i < m_config.ioThdCnt; i++)
		{
			boost::thread *thd = m_threads.create_thread(std::bind(&TcpServer::ProcessIO, shared_from_this()));
		}

		for (auto i = 0; i < m_config.wkThdCnt; i++)
		{
			boost::thread *thd = m_threads.create_thread(std::bind(&TcpServer::ProcessPacket, shared_from_this()));
		}

		m_threads.create_thread(std::bind(&TcpServer::SensePerform, shared_from_this()));

		//threads.join_all();
	}

	void TcpServer::Stop()
	{
	}

	size_t TcpServer::GetConnectionCount()
	{
		return m_connMgr.Count();
	}

	void TcpServer::OnConnect(ConnectResultPtr ret)
	{
		m_wks.post(std::bind(&PacketHandler::OnConnect, m_config.pkHandler, ret));
	}

	void TcpServer::OnRead(ConnectionPtr conn, PacketPtr &pack)
	{
		m_performSensor.AddReqCount();
		m_wks.post(std::bind(&PacketHandler::OnRead, m_config.pkHandler, conn, pack));
	}

	void TcpServer::OnWrite(WriteResultPtr &ret, WriteCallback callback)
	{
		m_performSensor.AddRspCount();
		m_wks.post(std::bind(callback, ret));
	}

	void TcpServer::OnError(ConnectionPtr conn, const ErrorType err, const ErrorCode &ec)
	{
		m_wks.post(std::bind(&PacketHandler::OnError, m_config.pkHandler, conn, err, ec));
	}

	void TcpServer::OnClose(ConnectionPtr conn)
	{
		std::string id;
		m_connMgr.Find(conn, id);
		m_connMgr.Remove(id);
		NetkitLog(InstNetkit, LogLevelInfo, "connection closed, id %s", id.c_str());

		m_wks.post(std::bind(&PacketHandler::OnClose, m_config.pkHandler, conn));
	}

	ConnectionPtr TcpServer::GetConnection(const std::string &id)
	{
		ConnectionPtr conn;
		m_connMgr.Find(id, conn);
		return conn;
	}

	std::string TcpServer::GetConnectionId(const ConnectionPtr &conn)
	{
		std::string id;
		m_connMgr.Find(conn, id);
		return id;
	}

	void TcpServer::GetAllConnection(std::vector<ConnectionPtr> &conns)
	{
		m_connMgr.GetAll(conns);
	}

	void TcpServer::ProcessIO()
	{
		NetkitLog(InstNetkit, LogLevelInfo, "io thread start");
		boost::asio::io_service::work work(m_ios);
		m_ios.run();
		NetkitLog(InstNetkit, LogLevelWarn, "io thread exit");
	}

	void TcpServer::ProcessPacket()
	{
		NetkitLog(InstNetkit, LogLevelInfo, "wk thread start");
		boost::asio::io_service::work work(m_wks);
		m_wks.run();
		NetkitLog(InstNetkit, LogLevelWarn, "wk thread exit");

	}

	void TcpServer::SensePerform()
	{
		while (true)
		{
			boost::this_thread::sleep_for(boost::chrono::seconds(1));
			m_performSensor.Report();
		}
	}

	void TcpServer::CreateSocket()
	{
		if (m_config.ssl)
		{
			m_sslSock.reset(new ssl_socket(m_ios, *m_sslCtx));
		}
		else
		{
			m_tcpSock.reset(new tcp_socket(m_ios));
		}
	}

	void TcpServer::Accept()
	{
		if (!m_acceptor.is_open())
		{
			NetkitLog(InstNetkit, LogLevelError, "acceptor not opened");
			return;
		}

		CreateSocket();
		m_acceptor.async_accept((m_config.ssl ? m_sslSock->lowest_layer() : *m_tcpSock), std::bind(&TcpServer::OnAccept, shared_from_this(), std::placeholders::_1));
	}

	void TcpServer::OnAccept(const ErrorCode &ec)
	{
		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "%s", ec.message().c_str());
			m_wks.post(std::bind(&PacketHandler::OnError, m_config.pkHandler, nullptr, AcceptError, ec));
			Accept();
			return;
		}
		
		boost::system::error_code e;
		auto ep = m_config.ssl ? m_sslSock->lowest_layer().remote_endpoint(e) : m_tcpSock->remote_endpoint(e);
		NetkitLog(InstNetkit, LogLevelInfo, "new connection arrived, remote addr %s:%d", ep.address().to_string().c_str(), (int)ep.port());

		if (m_config.ssl)
		{
			SslHandshake(m_sslSock);
			Accept();
			return;
		}
	
		std::string id(std::move(m_idGen.GenerateUuidString()));
		TcpConnectionPtr conn(m_config.ssl ? new TcpConnection(std::move(m_sslSock), m_config.bUseInternalUnzip) : new TcpConnection(std::move(m_tcpSock), m_config.bUseInternalUnzip));
		conn->SetHandler(shared_from_this());
		conn->Read();
		conn->SetId(id);
		m_connMgr.Add(id, conn);

		NetkitLog(InstNetkit, LogLevelInfo, "new connection established, conn id %s", id.c_str());

		ConnectResultPtr ret(new ConnectResult);
		ret->ec = ec;
		ret->conn = conn;
		OnConnect(ret);

		Accept();
	}

	void TcpServer::SslHandshake(SslSockPtr sslSock)
	{
		if (sslSock.get())
		{
			auto tmr = std::make_shared<boost::asio::deadline_timer>(m_wks, boost::posix_time::seconds(m_config.sslHandshakeTimeout));
			sslSock->async_handshake(boost::asio::ssl::stream_base::server, std::bind(&TcpServer::OnSslHandshake, shared_from_this(), std::placeholders::_1, sslSock, tmr));
			auto func = std::bind(&TcpServer::OnSslHandshakeTimeout, shared_from_this(), std::placeholders::_1, sslSock);
			tmr->async_wait(func);
		}
	}

	void TcpServer::OnSslHandshake(const boost::system::error_code &ec, SslSockPtr sslSock, TimerPtr tmr)
	{
		tmr->cancel();

		boost::system::error_code e;
		auto ep = sslSock->lowest_layer().remote_endpoint(e);

		if (ec)
		{
			NetkitLog(InstNetkit, LogLevelError, "ssl handshake fail, remote addr %s:%d, %s", ep.address().to_string().c_str(), (int)ep.port(), ec.message().c_str());
			m_wks.post(std::bind(&PacketHandler::OnError, m_config.pkHandler, nullptr, AcceptError, ec));
			return;
		}
		
		std::string id(std::move(m_idGen.GenerateUuidString()));
		TcpConnectionPtr conn(new TcpConnection(std::move(sslSock), m_config.bUseInternalUnzip));
		conn->SetHandler(shared_from_this());
		conn->Read();
		conn->SetId(id);
		m_connMgr.Add(id, conn);

		NetkitLog(InstNetkit, LogLevelInfo, "new ssl connection established, remote addr %s:%d, conn id %s", ep.address().to_string().c_str(), (int)ep.port(), id.c_str());

		ConnectResultPtr ret(new ConnectResult);
		ret->ec = ec;
		ret->conn = conn;
		OnConnect(ret);
	}

	void TcpServer::OnSslHandshakeTimeout(const boost::system::error_code &ec, SslSockPtr sslSock)
	{
		if (ec)
		{
			if (ec != boost::asio::error::operation_aborted)		// canceled
			{
				NetkitLog(InstNetkit, LogLevelError, "%s", ec.message().c_str());
			}

			return;
		}

		boost::system::error_code e;
		auto ep = sslSock->lowest_layer().remote_endpoint(e);
		NetkitLog(InstNetkit, LogLevelError, "ssl handshake timeout, close socket, remote addr %s:%d", ep.address().to_string().c_str(), (int)ep.port());
		
		boost::system::error_code ecd;
		sslSock->lowest_layer().close(ecd);
	}

	void TcpServer::AddTimer(size_t msInterval, Function callback)
	{
		auto tmr = std::make_shared<boost::asio::deadline_timer>(m_wks, boost::posix_time::milliseconds(msInterval));
		auto func = std::bind(&TcpServer::OnTimer, shared_from_this(), callback, tmr);
		tmr->async_wait(func);
	}

	void TcpServer::OnTimer(Function callback, TimerPtr tmr)
	{
		callback();
	}

	//////////////////////////////////////////////////////////

	void TcpConnectionManager::Add(const std::string &id, ConnectionPtr conn)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		if (id.empty())
		{
			return;
		}

		m_conns[id] = conn;
	}

	void TcpConnectionManager::Remove(ConnectionPtr conn)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		for (auto &itr : m_conns)
		{
			if (itr.second == conn)
			{
				m_conns.erase(itr.first);
				break;
			}
		}
	}

	void TcpConnectionManager::Remove(const std::string &id)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		m_conns.erase(id);
	}

	bool TcpConnectionManager::Find(const std::string &id, ConnectionPtr &conn)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		std::map<std::string, ConnectionPtr>::iterator itr = m_conns.find(id);
		if (itr != m_conns.end())
		{
			conn = itr->second;
			return true;
		}

		return false;
	}

	bool TcpConnectionManager::Find(const ConnectionPtr &conn, std::string &id)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		for (auto &itr : m_conns)
		{
			if (itr.second == conn)
			{
				id = itr.first;
				return true;
			}
		}

		return false;
	}

	void TcpConnectionManager::GetAll(std::vector<ConnectionPtr> &conns)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		for (auto &itr : m_conns)
		{
			conns.push_back(itr.second);
		}
	}

	void TcpConnectionManager::RemoveAll()
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		m_conns.clear();
	}

	size_t TcpConnectionManager::Count()
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		return m_conns.size();
	}

}

