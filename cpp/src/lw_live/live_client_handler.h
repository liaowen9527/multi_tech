#pragma once
#include "lw_live.h"
#include "client_handler.h"
#include "interaction.h"

namespace lw_live {
	
	class LiveClientHandler : public lw_client::ClientHandler
	{
	public:
		LiveClientHandler();
		~LiveClientHandler();

	public:
		void SetInteraction(Interaction* interaction);

	protected:
		virtual void OnConnected();
		virtual void OnDisconnected(int nErrCode, const std::string& err_msg);

		virtual void OnReceived(const char* str, int nLen);

		virtual void OnError(int nErrCode, const std::string& err_msg);

		virtual void OnLoginResult(bool bValue);

	protected:
		bool UseInteraction(lw_util::Interface& intf);

	protected:
		Interaction* m_interaction;

		DestinationPtr m_currDest;
	};

	typedef std::shared_ptr<LiveClientHandler> LiveClientHandlerPtr;

}


