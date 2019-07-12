#include "ssh_client.h"
#include "ssh_client_param.h"
#include "lw_util/log/logger_define.h"

namespace lw_client {

	SshClient::SshClient()
	{
		m_clientPtr = std::make_shared<PuttySsh>();
		m_clientPtr->SetNotify(this);
	}

	SshClient::~SshClient()
	{

	}

	bool SshClient::AsyncConnect(ClientParam* pConnParam)
	{
		EasyLog(InstClient, LOG_DEBUG, "begin");

		SshClientParam* pSshParam = dynamic_cast<SshClientParam*>(pConnParam);
		if (nullptr == pSshParam)
		{
			return false;
		}

		if (!__super::AsyncConnect(pConnParam))
		{
			return false;
		}

		switch (pSshParam->GetSshVerion())
		{
		case ssh1:
			EasyLog(InstClient, LOG_INFO, "ssh1 to ip:%s, port:%d", pConnParam->GetHost().c_str(), pConnParam->GetPort());
			m_clientPtr->SetSshVer(ssh_v1);
			break;
		case ssh2:
			EasyLog(InstClient, LOG_INFO, "ssh2 to ip:%s, port:%d", pConnParam->GetHost().c_str(), pConnParam->GetPort());
			m_clientPtr->SetSshVer(ssh_v2);
			break;
		}

		m_clientPtr->SetUserName(pConnParam->GetUsername());
		m_clientPtr->SetPassword(pConnParam->GetPassword());
		m_clientPtr->SetLoginConfirm(pConnParam->IsConfirmLogin());

		std::string strKeyFile = pSshParam->GetPriKeyFile();
		m_clientPtr->SetKeyfile(strKeyFile);
		if (!strKeyFile.empty())
		{
			EasyLog(InstClient, LOG_INFO, "ssh use private key file.");
		}

		return m_clientPtr->Connect(pConnParam->GetHost(), pConnParam->GetPort());
	}

	bool SshClient::AsyncDisConnect()
	{
		EasyLog(InstClient, LOG_INFO, "begin");
		if (!__super::AsyncDisConnect())
		{
			return false;
		}

		return m_clientPtr->DisConnect();
	}

	bool SshClient::Send(const std::string& data)
	{
		return m_clientPtr->Send(data);
	}

	void SshClient::OnConnected()
	{
		EasyLog(InstClient, LOG_DEBUG, "begin");
		m_connStatus.connected();

		if (m_handler)
		{
			m_handler->OnConnected();
		}
	}

	void SshClient::OnDisconnected(int nErrCode, const std::string& err_msg)
	{
		EasyLog(InstClient, LOG_INFO, "message:%s", err_msg.c_str());
		m_connStatus.disconnected();

		if (m_handler)
		{
			m_handler->OnDisconnected(nErrCode, err_msg);
		}
	}

	void SshClient::OnReceived(const char* str, int nLen)
	{
		if (m_handler)
		{
			m_handler->OnReceived(str, nLen);
		}
	}

	void SshClient::OnError(int nErrCode, const std::string& err_msg)
	{
		EasyLog(InstClient, LOG_ERROR, "err_code:%d, err_msg:%s", nErrCode, err_msg.c_str());
		if (m_handler)
		{
			m_handler->OnError(nErrCode, err_msg);
		}
	}

	void SshClient::OnLoginResult(bool bValue)
	{
		EasyLog(InstClient, LOG_INFO, "is login: %d", bValue);
		if (m_handler)
		{
			m_handler->OnLoginResult(bValue);
		}
	}

}