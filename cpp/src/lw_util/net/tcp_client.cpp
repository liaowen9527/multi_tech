#include "tcp_client.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/lexical_cast.hpp>
#include "log/easylog.h"

namespace netkit {

	TcpClient::TcpClient()
	{

	}

	TcpClient::~TcpClient()
	{

	}

	void TcpClient::Config(const ClientConfig& cfg)
	{
		m_config = cfg;
	}

	bool TcpClient::Run()
	{
		return true;
	}

	void TcpClient::Stop()
	{

	}

	void TcpClient::Connect(ConnectCallback callback)
	{
		CreateSocket();

		std::lock_guard<SocketLock> lock(m_sockLock);

		boost::asio::ip::tcp::resolver resolver(m_ios);
		boost::asio::ip::tcp::resolver::query query(m_config.svrAddr, std::to_string(m_config.port));
		
		boost::system::error_code ec;
		boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query, ec);

		boost::asio::ip::tcp::endpoint endpoint;
		if (ec)
		{
			EasyLog(InstNet, LOG_ERROR, "resolve server address fail, %s,this = %p", ec.message().c_str(), this);
		}
		else
		{
			endpoint = *iter;
			EasyLog(InstNet, LOG_INFO, "resolve server address succ, %s:%d,this = %p", endpoint.address().to_string().c_str(), (int)endpoint.port(), this);
		}

		if (m_config.ssl)
		{
			assert(m_sslSock.get());
			m_sslSock->lowest_layer().async_connect(endpoint, std::bind(&TcpClient::OnConnect, shared_from_this(), std::placeholders::_1, m_strand.wrap(callback)));
		}
		else
		{
			assert(m_tcpSock.get());
			m_tcpSock->async_connect(endpoint, std::bind(&TcpClient::OnConnect, shared_from_this(), std::placeholders::_1, m_strand.wrap(callback)));
		}
	}

	void TcpClient::Close()
	{

	}

	void TcpClient::Write(const BinaryDataPtr& data, WriteCallback callback /*= nullptr*/)
	{
		Message msg;
		msg.data = data;
		msg.callback = callback;

		std::lock_guard<SocketLock> lock(m_sockLock);
		m_msgQueue.push(msg);
	}

	bool TcpClient::Connected()
	{
		return m_connected;
	}

	std::string TcpClient::GetLocalAddress()
	{

	}

	void TcpClient::CreateSocket()
	{
		std::lock_guard<SocketLock> lock(m_sockLock);

		if (m_config.ssl)
		{
			m_sslSock = std::make_shared<ssl_socket>(m_ios, *m_sslCtx);
		}
		else
		{
			m_tcpSock = std::make_shared<tcp::socket>(m_ios);
		}
	}

	void TcpClient::SslHandshake(std::shared_ptr<ssl_socket> sslSock, ConnectCallback callback)
	{
		if (sslSock && sslSock.get())
		{
			auto tmr = std::make_shared<deadline_timer>(m_wks, boost::posix_time::seconds(m_config.sslHandshakeTimeout));
			sslSock->async_handshake(boost::asio::ssl::stream_base::client, m_strand.wrap(std::bind(&TcpClient::OnSslHandshake, shared_from_this(), std::placeholders::_1, sslSock, tmr, callback)));

			auto func = std::bind(&TcpClient::OnSslHandshakeTimeout, shared_from_this(), std::placeholders::_1, sslSock);
			tmr->async_wait(func);
		}
	}

	void TcpClient::Read()
	{

	}

	void TcpClient::OnConnect(const error_code &ec, ConnectCallback callback)
	{
		if (ec)
		{
			EasyLog(InstNet, LOG_ERROR, "connect fail, %s,this = %p", ec.message().c_str(), this);
		}
		else
		{
			if (m_config.ssl)
			{
				std::lock_guard<SocketLock> lock(m_sockLock);
				m_strand.post(std::bind(&TcpClient::SslHandshake, shared_from_this(), m_sslSock, callback));
				return;
			}
			else
			{
				EasyLog(InstNet, LOG_INFO, "Connection established,this = %p", this);
				m_connected = true;
				Read();
			}
		}
	}

	void TcpClient::OnSslHandshake(const error_code &ec, SslSocketPtr sslSock, DeadlineTimerPtr tmr, ConnectCallback callback)
	{
		tmr->cancel();

		if (ec)
		{
			EasyLog(InstNet, LOG_ERROR, "ssl handshake fail, %s", ec.message().c_str());
		}
		else
		{
			EasyLog(InstNet, LOG_INFO, "ssl connection established");
			m_connected = true;
			Read();
		}
	}

	void TcpClient::OnSslHandshakeTimeout(const error_code &ec, SslSocketPtr sslSock)
	{
		if (ec)
		{
			if (ec != boost::asio::error::operation_aborted)		// canceled
			{
				EasyLog(InstNet, LOG_ERROR, "%s", ec.message().c_str());
			}

			return;
		}

		EasyLog(InstNet, LOG_ERROR, "ssl handshake timeout, close socket");

		std::lock_guard<SocketLock> lock(m_sockLock);
		boost::system::error_code ecd;
		sslSock->lowest_layer().close(ecd);
	}

}

