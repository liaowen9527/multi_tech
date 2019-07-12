#include "live_client_handler.h"
#include "std/string_convert.h"


namespace lw_live {

	
	LiveClientHandler::LiveClientHandler()
	{

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
		DisplayPtr displayPtr = m_interaction->GetDisplay();
		if (displayPtr)
		{
			displayPtr->OnDisconnected(nErrCode, StringConverter::utf_to_utf(err_msg));
		}
	}

	void LiveClientHandler::OnReceived(const char* str, int nLen)
	{
		DisplayPtr displayPtr = m_interaction->GetDisplay();
		
	}

	void LiveClientHandler::OnError(int nErrCode, const std::string& err_msg)
	{

	}

	void LiveClientHandler::OnLoginResult(bool bValue)
	{

	}

}


