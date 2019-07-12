#pragma once
#include <string>
#include <memory>
#include "lw_live.h"
#include "dest_loop.h"
#include "client_handler.h"

namespace lw_live {

	class LWLive_API DestLoopHandler : public lw_client::ClientHandler
	{
	public:
		DestLoopHandler();
		virtual ~DestLoopHandler();

	public:
		DestinationPtr GetNext();
		DestinationPtr GetCurrent();

		bool CanTryNext();
		void SetTryNext(bool val);

	protected:
		virtual void OnConnected();
		virtual void OnDisconnected(int nErrCode, const std::string& err_msg);

		virtual void OnReceived(const char* str, int nLen);

		virtual void OnError(int nErrCode, const std::string& err_msg);

		virtual void OnLoginResult(bool bValue);

	protected:
		DestLoopPtr m_destination;
		bool m_bTryNext;
	};


}


