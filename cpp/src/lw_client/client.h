#pragma once
#include <string>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "lw_client.h"
#include "status.h"
#include "client_param.h"
#include "client_handler.h"

namespace lw_client {

	class LWClient_API Client
	{
	public:
		Client();
		virtual ~Client();

	public:
		ClientHandlerPtr GetHandler();
		void SetHandler(ClientHandlerPtr handle);

	public:
		bool IsConnected();
		virtual bool Connect(ClientParam* pConnParam);
		virtual bool AsyncConnect(ClientParam* pConnParam);

		virtual bool DisConnect();
		virtual bool AsyncDisConnect();

		virtual bool Send(const std::string& data) = 0;

	protected:
		ClientHandlerPtr m_handler;
		AsyncStatus m_connStatus;
	};
	typedef std::shared_ptr<Client> ClientPtr;

}

