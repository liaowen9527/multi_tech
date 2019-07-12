#pragma once
#include <string>
#include <memory>
#include "lw_client.h"

namespace lw_client {

	class LWClient_API ClientParam
	{
	protected:
		ClientParam(const std::string& host, int port);
		virtual ~ClientParam();

	public:
		std::string GetHost() const;
		void SetHost(const std::string& str);

		int GetPort() const;
		void SetPort(int port);

		std::string GetUsername() const;
		void SetUsername(const std::string& str);

		std::string GetPassword() const;
		void SetPassword(const std::string& str);

		bool IsConfirmLogin();
		void SetLoginConfirm(bool bConfirm);

	protected:
		std::string m_host;
		int m_port;
		std::string m_username;
		std::string m_password;

		bool m_bLoginConfirm;
	};
	typedef std::shared_ptr<ClientParam> ClientParamPtr;

}



