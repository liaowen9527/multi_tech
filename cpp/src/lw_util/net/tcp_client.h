#pragma once
#include "netkit.h"
#include <queue>
#include <mutex>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ssl.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;

namespace netkit
{
	typedef ssl::stream<tcp::socket> ssl_socket;
	typedef std::shared_ptr<ssl_socket> SslSocketPtr;
	typedef std::shared_ptr<deadline_timer> DeadlineTimerPtr;

	class TcpClient : public Client, public std::enable_shared_from_this<TcpClient>
	{
	public:
		TcpClient();
		virtual ~TcpClient();

	public:
		virtual void Config(const ClientConfig& cfg);

		virtual bool Run();
		virtual void Stop();

		virtual void Connect(ConnectCallback callback);
		virtual void Close();

		virtual void Write(const BinaryDataPtr& data, WriteCallback callback = nullptr);

	public:
		virtual bool Connected();

		virtual std::string GetLocalAddress();

	protected:
		void CreateSocket();
		void SslHandshake(std::shared_ptr<ssl_socket> sslSock, ConnectCallback callback);

		void Read();

	protected:
		void OnConnect(const error_code &ec, ConnectCallback callback);
		void OnSslHandshake(const error_code &ec, SslSocketPtr sslSock, DeadlineTimerPtr tmr, ConnectCallback callback);
		void OnSslHandshakeTimeout(const error_code &ec, SslSocketPtr sslSock);

		void OnRead(PacketPtr pack);
		void OnError(ErrorType err, error_code &ec);
		void OnClose();

	protected:
		std::queue<Message> m_msgQueue;
		ClientConfig m_config;
		std::atomic_bool m_connected;

		std::vector<boost::thread*> m_threads;
		std::mutex m_threadLock;

	protected:
		typedef std::mutex SocketLock;
		SocketLock m_sockLock;

		boost::asio::io_service m_ios;
		boost::asio::io_service m_wks;
		boost::shared_ptr<ssl::context> m_sslCtx;
		std::shared_ptr<tcp::socket> m_tcpSock;
		std::shared_ptr<ssl_socket> m_sslSock;

		boost::asio::streambuf m_buffer;
		std::string m_headDelim;
		boost::asio::io_service::strand m_strand;
	};

	typedef std::shared_ptr<TcpClient> TcpClientPtr;

}