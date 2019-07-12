#pragma once
#include "client.h"
#include "lw_putty/putty_ssh.h"

namespace lw_client {

	class SshClient : public Client, public PuttyClientNotify
	{
	public:
		SshClient();
		~SshClient();

	public:
		virtual bool AsyncConnect(ClientParam* pConnParam);
		virtual bool AsyncDisConnect();

		virtual bool Send(const std::string& data);

	protected:
		virtual void OnConnected();
		virtual void OnDisconnected(int nErrCode, const std::string& err_msg);

		virtual void OnReceived(const char* str, int nLen);

		virtual void OnError(int nErrCode, const std::string& err_msg);

		virtual void OnLoginResult(bool bValue);

	protected:
		PuttySshPtr m_clientPtr;
	};

	typedef std::shared_ptr<SshClient> PuttySshClientPtr;

}

