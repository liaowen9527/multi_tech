#pragma once
#include <functional>
#include <memory>
#include "define.h"

namespace netkit {

#ifndef interface
#define interface struct
#endif

	typedef std::shared_ptr<Connection> ConnectionPtr;

	struct NetkitApi ConnectResult
	{
		std::shared_ptr<Connection> conn;
		std::error_code ec;
	};

	struct NetkitApi WriteResult
	{
		ConnectionPtr conn;
		std::error_code ec;
	};

	typedef std::function<void(ConnectResultPtr)> ConnectCallback;
	typedef std::function<void(WriteResultPtr)> WriteCallback;

	interface NetkitApi PacketHandler
	{
		virtual void OnConnect(ConnectResultPtr ret) = 0;
		virtual void OnRead(ConnectionPtr conn, PacketPtr &pack) = 0;
		virtual void OnWrite(WriteResultPtr &ret, WriteCallback callback) = 0;
		virtual void OnError(ConnectionPtr conn, const ErrorType err, const std::error_code &ec) = 0;
		virtual void OnClose(ConnectionPtr conn) = 0;
	};
	

}

