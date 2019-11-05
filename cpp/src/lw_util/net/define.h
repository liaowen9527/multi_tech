#pragma once
#include <string>
#include <memory>

#ifndef NetkitApi
#define NetkitApi
#endif

namespace netkit {

#ifndef interface
#define interface struct
#endif

#ifdef boost_ec
	typedef boost::system::error_code ErrorCode;
#else
	typedef std::error_code ErrorCode;
#endif

	interface Connection;
	interface Client;
	interface Server;
	interface PacketHandler;
	interface ConnectionManager;

	struct Packet;
	struct WriteResult;
	struct ConnectResult;

	typedef std::string BinaryData;
	typedef std::shared_ptr<char> CharPtr;
	typedef std::shared_ptr<BinaryData> BinaryDataPtr;
	typedef std::shared_ptr<Connection> ConnectionPtr;
	typedef std::shared_ptr<ConnectionManager> ConnectionManagerPtr;
	typedef std::shared_ptr<PacketHandler> PackHandlerPtr;
	typedef std::shared_ptr<Server> ServerPtr;
	typedef std::shared_ptr<Client> ClientPtr;
	typedef std::shared_ptr<Packet> PacketPtr;
	typedef std::shared_ptr<ConnectResult> ConnectResultPtr;
	typedef std::shared_ptr<WriteResult> WriteResultPtr;

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

}

