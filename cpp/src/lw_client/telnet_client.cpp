#include "telnet_client.h"
#include "telnet_client_param.h"
#include "lw_util/log/easylog.h"

namespace lw_client {

	TelnetClient::TelnetClient()
	{
		m_clientPtr = std::make_shared<PuttyTelnet>();
		m_clientPtr->SetNotify(this);
	}

	TelnetClient::~TelnetClient()
	{

	}

	bool TelnetClient::AsyncConnect(ClientParam* pConnParam)
	{
		EasyLog(InstClient, LOG_DEBUG, "begin");
		if (!__super::AsyncConnect(pConnParam))
		{
			return false;
		}

		m_clientPtr->SetLoginConfirm(pConnParam->IsConfirmLogin());

		EasyLog(InstClient, LOG_INFO, "connect to ip:%s, port:%d", pConnParam->GetHost().c_str(), pConnParam->GetPort());
		return m_clientPtr->Connect(pConnParam->GetHost(), pConnParam->GetPort());
	}

	bool TelnetClient::AsyncDisConnect()
	{
		EasyLog(InstClient, LOG_INFO, "begin");
		__super::AsyncDisConnect();
		return m_clientPtr->DisConnect();
	}

	bool TelnetClient::Send(const std::string& data)
	{
		return m_clientPtr->Send(data);
	}

	void TelnetClient::OnConnected()
	{
		EasyLog(InstClient, LOG_DEBUG, "begin");
		m_connStatus.connected();

		if (m_handler)
		{
			m_handler->OnConnected();
		}
	}

	void TelnetClient::OnDisconnected(int nErrCode, const std::string& err_msg)
	{
		EasyLog(InstClient, LOG_INFO, "message:%s", err_msg.c_str());
		m_connStatus.disconnected();

		if (m_handler)
		{
			m_handler->OnDisconnected(nErrCode, err_msg);
		}
	}

	void TelnetClient::OnReceived(const char* str, int nLen)
	{
		if (m_handler)
		{
			m_handler->OnReceived(str, nLen);
		}
	}

	void TelnetClient::OnError(int nErrCode, const std::string& err_msg)
	{
		EasyLog(InstClient, LOG_INFO, "err_code:%d, err_msg:%s", nErrCode, err_msg.c_str());
		if (m_handler)
		{
			m_handler->OnError(nErrCode, err_msg);
		}
	}

}