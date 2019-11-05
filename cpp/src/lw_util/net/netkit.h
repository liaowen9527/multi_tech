#pragma once
#include <vector>
#include "define.h"
#include "config.h"
#include "callback.h"

namespace netkit {

	struct NetkitApi Message
	{
		BinaryDataPtr data;
		WriteCallback callback;
	};

	interface NetkitApi Connection
	{
		virtual bool Connected() = 0;
		virtual void Close() = 0;

		virtual void Write(const BinaryDataPtr& data, WriteCallback callback = nullptr) = 0;

		virtual std::string GetLocalAddress() = 0;
	};

	interface NetkitApi Client : public Connection
	{
		virtual void Config(const ClientConfig& cfg) = 0;
		virtual void SetCallback() = 0;

		virtual bool Run() = 0;
		virtual void Stop() = 0;

		virtual void Connect(ConnectCallback callback) = 0;
	};

	interface NetkitApi Server
	{
		virtual void Config(const ServerConfig& cfg) = 0;

		virtual bool Start() = 0;
		virtual void Stop() = 0;

		virtual size_t GetConnectionCount() = 0;
		virtual void GetAllConnection(std::vector<ConnectionPtr>& conns) = 0;
	};

}

