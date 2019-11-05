#pragma once
#include <string>
#include <vector>
#include "lw_util.h"
#include <atomic>
#include <memory>
#include "thread/safe/dictionary.hpp"

namespace netkit {

#define NetkitApi LWUtil_API

#ifndef interface
#define interface struct
#endif

	interface ErrorCode;
	interface Connection;
	interface Client;
	interface Server;
	interface PacketHandler;
	struct Packet;
	struct WriteResult;
	struct ConnectResult;
	class ConnectionManager;

	typedef std::shared_ptr<std::string> BinaryDataPtr;
	typedef std::shared_ptr<ErrorCode> ErrorCodePtr;
	typedef std::shared_ptr<Connection> ConnectionPtr;
	typedef std::shared_ptr<ConnectionManager> ConnectionManagerPtr;
	typedef std::shared_ptr<PacketHandler> PackHandlerPtr;
	typedef std::shared_ptr<Server> ServerPtr;
	typedef std::shared_ptr<Client> ClientPtr;
	typedef std::shared_ptr<Packet> PacketPtr;
	typedef std::shared_ptr<ConnectResult> ConnectResultPtr;
	typedef std::shared_ptr<WriteResult> WriteResultPtr;

	typedef std::function<void()> Function;
	typedef std::function<void(ConnectResultPtr)> ConnectCallback;
	typedef std::function<void(WriteResultPtr)> WriteCallback;

	enum ErrorType
	{
		EndPointError,
		ListenError,
		AcceptError,
		ConnectError,
		ReadError,
		WriteError,
		PacketError,
	};

	interface ErrorCode
	{
		virtual int value() const = 0;
		virtual std::string message() const = 0;
		virtual operator bool() = 0;
	};

	struct NetkitApi ConnectResult
	{
		ConnectionPtr conn;
		ErrorCodePtr ec;

		bool Succ();
	};

	struct NetkitApi WriteResult
	{
		ConnectionPtr conn;
		ErrorCodePtr ec;

		bool Succ();
	};

	struct NetkitApi ClientConfig
	{
		std::string proto;					// tcp or websocket
		std::string svrAddr;				// server ip address
		unsigned short port;				// server port
		bool ssl;								// use ssl or not, default not
		bool sslVerifyCert;				// ssl verify certificate or not, default not
		std::string sslCertFile;			// ssl verify certificate file path
		size_t sslHandshakeTimeout;	// ssl handshake timeout in seconds
		size_t ioThdCnt;					// io thread count, default 1
		size_t wkThdCnt;					// packet handle thread count, default 1
		PackHandlerPtr pkHandler;		// packet handler
		bool tcpKeepaliveEnable;		// tcp keep alive enable
		int tcpKeepaliveTimeout;		// timeout of tcp keep alive
		int tcpKeepaliveInterval;		// interval of tcp keep alive
		int tcpKeepaliveCount;			// count of tcp keep alive

		ClientConfig();
		bool CheckValid(std::string& errMsg) const;
	};

	struct NetkitApi Endpoint
	{
		std::string addr;				// ip address
		unsigned short port;			// port

		Endpoint();
		bool CheckValid(std::string& errMsg) const;
	};

	struct NetkitApi ServerConfig
	{
		std::string proto;					// tcp or websocket
		std::string addr;					// ip address to listen
		unsigned short port;				// port to listen
		bool ssl;								// use ssl or not, default not
		std::string sslCertFile;			// ssl certificate file path
		std::string sslKeyFile;			// ssl key file path
		size_t sslHandshakeTimeout;	// ssl handshake timeout in seconds
		size_t ioThdCnt;					// io thread count, default 1
		size_t wkThdCnt;					// packet handle thread count, default 1
		PackHandlerPtr pkHandler;		// packet handler
		bool bUseInternalUnzip;			// 
		bool reuseAddr;					// reuse bind addr

		ServerConfig();
		bool CheckValid(std::string& errMsg) const;
	};

	struct NetkitApi WriteInfo
	{
		BinaryDataPtr data;
		WriteCallback callback;
	};

	struct NetkitApi ClientStatistics
	{
		int unsentPackCount;
		size_t unsentDataSize;
		size_t sentDataSize;
		size_t nSentDataSizeRate;

		ClientStatistics();
	};

	interface NetkitApi Connection
	{
		virtual void SetHandler(PackHandlerPtr handler) = 0;
		virtual bool Connected() = 0;
		virtual void Read() = 0;
		virtual void Write(const BinaryDataPtr& data, WriteCallback callback) = 0;
		virtual void Close() = 0;
		virtual std::string GetLocalAddress() = 0;
		virtual time_t GetLastRequestTime();
		virtual void SetLastRequestTime(time_t tim);
		virtual bool GetTrustful();
		virtual void SetTrustful(bool trustful);
	};

	interface NetkitApi Client : public Connection
	{
		virtual bool Run() = 0;
		virtual void Stop() = 0;
		virtual void Connect(ConnectCallback callback) = 0;
		virtual void GetStatisticsInfo(ClientStatistics& stat) = 0;
		virtual size_t GetSendQueueDataSize() = 0;
		virtual int GetSendQueuePkgCount() = 0;
		virtual void AddTimer(size_t msInterval, Function callback) = 0;
		virtual void SetConnectionConfig(const Endpoint& cfg) = 0;
	};

	interface NetkitApi Server
	{
		virtual bool Start() = 0;
		virtual void Stop() = 0;
		virtual size_t GetConnectionCount() = 0;
		virtual ConnectionPtr GetConnection(const std::string& id) = 0;
		virtual std::string GetConnectionId(const ConnectionPtr& conn) = 0;
		virtual void GetAllConnection(std::vector<ConnectionPtr>& conns) = 0;
		virtual void AddTimer(size_t msInterval, Function callback) = 0;
	};

	class NetkitApi ConnectionManager
	{
	public:
		void Add(const std::string& id, ConnectionPtr conn);
		void Remove(ConnectionPtr conn);
		void Remove(const std::string& id);
		bool Find(const std::string& id, ConnectionPtr& conn);
		bool Find(const ConnectionPtr& conn, std::string& id);
		void GetAll(std::vector<ConnectionPtr>& conns);
		void RemoveAll();
		size_t Count();

	protected:
		thread_safe::dictionary<std::string, ConnectionPtr> m_conns;
	};

	interface NetkitApi PacketHandler
	{
		virtual void OnConnect(ConnectResultPtr ret) = 0;
		virtual void OnRead(ConnectionPtr conn, PacketPtr& pack) = 0;
		virtual void OnWrite(WriteResultPtr& ret, WriteCallback callback) = 0;
		virtual void OnError(ConnectionPtr conn, const ErrorType err, const ErrorCode& ec) = 0;
		virtual void OnClose(ConnectionPtr conn) = 0;
	};

	class NetkitApi Factory
	{
	public:
		static ClientPtr CreateClient(const ClientConfig& cfg);
		static ServerPtr CreateServer(const ServerConfig& cfg);
	};

}
