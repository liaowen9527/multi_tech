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
		Interface intf;
		if (!UseInteraction(intf)) return;

		DisplayPtr displayPtr = m_interaction->GetDisplay();
		if (displayPtr)
		{
			displayPtr->OnConnected();
		}
	}

	void LiveClientHandler::OnDisconnected(int nErrCode, const std::string& err_msg)
	{
		Interface intf;
		if (!UseInteraction(intf)) return;

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
		Interface intf;
		if (!UseInteraction(intf)) return;

		m_interaction->SetLiveSucc(true);

		m_interaction->WriteData(str, nLen);
	}

	void LiveClientHandler::OnError(int nErrCode, const std::string& err_msg)
	{
		Interface intf;
		if (!UseInteraction(intf)) return;

		m_interaction->WriteData(err_msg.c_str(), err_msg.length());

		std::string str = "\r\n";
		m_interaction->WriteData(str.c_str(), str.length());
	}

	void LiveClientHandler::OnLoginResult(bool bValue)
	{

	}

	bool LiveClientHandler::UseInteraction(lw_util::Interface& intf)
	{
		InterfaceSvrPtr svrPtr = InterfaceSvr::GetSafeSvr(m_interaction);
		if (nullptr == svrPtr)
		{
			return false;
		}

		intf.m_userdata = this;

		return intf.Open(svrPtr);
	}

}


