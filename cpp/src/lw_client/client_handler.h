#pragma once
#include <string>
#include <memory>
#include <mutex>
#include "lw_client.h"

namespace lw_client {

	class LWClient_API ClientHandler
	{
	public:
		virtual void OnConnected();
		virtual void OnDisconnected(int nErrCode, const std::string& err_msg);

		virtual void OnReceived(const char* str, int nLen);

		virtual void OnError(int nErrCode, const std::string& err_msg);

		virtual void OnLoginResult(bool bValue);
	};

	typedef std::shared_ptr<ClientHandler> ClientHandlerPtr;

}

