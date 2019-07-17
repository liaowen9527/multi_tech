#include "live_client_handler.h"
#include "std/string_convert.h"

using namespace lw_util;

namespace lw_live {

	
	LiveClientHandler::LiveClientHandler()
	{
		m_interaction = nullptr;
	}

	LiveClientHandler::~LiveClientHandler()
	{

	}

	void LiveClientHandler::SetInteraction(Interaction* interaction)
	{
		m_interaction = interaction;
	}

	void LiveClientHandler::OnConnected()
	{
		DisplayPtr displayPtr = m_interaction->GetDisplay();
		if (displayPtr)
		{
			displayPtr->OnConnected();
		}
	}

	void LiveClientHandler::OnDisconnected(int nErrCode, const std::string& err_msg)
	{
		DestinationPtr destPtr = m_interaction->GetDestination();
		if (nullptr == destPtr)
		{
			return;
		}

		if (!m_interaction->Connect(true))
		{
			DisplayPtr displayPtr = m_interaction->GetDisplay();
			if (displayPtr)
			{
				displayPtr->OnDisconnected(nErrCode, StringConverter::utf_to_utf(err_msg));
			}
		}
	}

	void LiveClientHandler::OnReceived(const char* str, int nLen)
	{
		m_interaction->SetLiveSucc(true);

		m_interaction->WriteData(str, nLen);
	}

	void LiveClientHandler::OnError(int nErrCode, const std::string& err_msg)
	{
		m_interaction->WriteData(err_msg.c_str(), err_msg.length());
	}

	void LiveClientHandler::OnLoginResult(bool bValue)
	{

	}

}


