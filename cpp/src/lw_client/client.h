#pragma once
#include <string>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "lw_client.h"
#include "status.h"
#include "parameter.h"
#include "defines.h"
#include "notify.h"

namespace lw_client {

	class LWClient_API Client
	{
	public:
		Client();
		virtual ~Client();

	public:
		void SetNotify(Notify* pNotify);

		virtual void SetTermSize(int nWidth, int nHeight);

	public:
		bool IsConnected();
		virtual bool Connect(Parameter* pConnParam);
		virtual bool AsyncConnect(Parameter* pConnParam);

		virtual bool DisConnect();
		virtual bool AsyncDisConnect();

		virtual bool Send(const std::string& data) = 0;

	protected:
		NotifyEx* m_pNotify;
		AsyncStatus m_connStatus;
	};
	typedef std::shared_ptr<Client> ClientPtr;

}

