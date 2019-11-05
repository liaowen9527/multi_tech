#pragma once
#include <mutex>
#include <atomic>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ssl.hpp>
#include "thread/safe/dictionary.hpp"

#include "netkit.h"

namespace netkit {

	class TcpConnection;
	class TcpServer;
	typedef std::recursive_mutex SocketLock;
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::shared_ptr<TcpServer> TcpServerPtr;
	typedef boost::asio::ip::tcp::socket tcp_socket;
	typedef boost::asio::ssl::stream<tcp_socket> ssl_socket;
	typedef std::shared_ptr<tcp_socket> TcpSockPtr;
	typedef std::shared_ptr<ssl_socket> SslSockPtr;
	typedef std::shared_ptr<boost::asio::deadline_timer> TimerPtr;

	class TcpConnection : public Connection,
		public std::enable_shared_from_this<TcpConnection>
	{
	public:
		TcpConnection(TcpSockPtr socket, bool bUseInternalUnzip);
		TcpConnection(SslSockPtr socket, bool bUseInternalUnzip);
		~TcpConnection();

	public:
		virtual bool Connected();
		virtual void Read();
		virtual void Write(const BinaryDataPtr &data, WriteCallback callback);
		virtual void Close();

	public:
		virtual void SetHandler(PackHandlerPtr handler);
		
		virtual std::string GetId();
		virtual void SetId(const std::string &id);
		
		virtual std::string GetLocalAddress();

		virtual time_t GetLastRequestTime();
		virtual void SetLastRequestTime(time_t tim);

		virtual bool GetTrustful();
		virtual void SetTrustful(bool trustful);
		
	protected:
		virtual void OnRead(PacketPtr pack);
		virtual void OnClose();
		virtual void OnError(const ErrorType err, const ErrorCode &ec);

	protected:
		void OnReadHead(ErrorCode ec, std::size_t byteCnt);
		void OnReadBody(ErrorCode ec, std::size_t byteCnt, PacketPtr pack, size_t bodyDataSize);
		void OnWriteData(ErrorCode ec, std::size_t byteCnt, WriteCallback callback, BinaryDataPtr data);
		
		bool ExtractHeadFromBuffer(std::string &head);
		bool ExtractBodyFromBuffer(PacketPtr &pack, size_t &extractedSize);
		void WriteDirectly(const BinaryDataPtr &data, WriteCallback callback);

	protected:
		TcpSockPtr m_tcpSock;
		SslSockPtr m_sslSock;
		PackHandlerPtr m_packHandler;
		SocketLock m_sockLock;
		std::atomic_bool m_connected;
		boost::asio::streambuf m_buffer;
		static std::string m_headDelim;
		std::string m_id;
		std::queue<WriteInfo> m_writeQueue;
		std::atomic<int64_t> m_lastReqTime;
		std::atomic<bool> m_trustful;
		boost::asio::io_service::strand m_strand;
		bool m_bUseInternalUnzip;
	};

	class TcpConnectionManager : public ConnectionManager
	{
	public:
		virtual void Add(const std::string &id, ConnectionPtr conn);
		virtual void Remove(ConnectionPtr conn);
		virtual void Remove(const std::string &id);
		virtual bool Find(const std::string &id, ConnectionPtr &conn);
		virtual bool Find(const ConnectionPtr &conn, std::string &id);
		virtual void GetAll(std::vector<ConnectionPtr> &conns);
		virtual void RemoveAll();
		virtual size_t Count();

	protected:
		thread_safe::dictionary<std::string, ConnectionPtr> m_conns;
	};

}