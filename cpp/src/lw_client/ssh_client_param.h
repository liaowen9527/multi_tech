#pragma once
#include <string>
#include <memory>
#include "lw_client.h"
#include "client_param.h"

namespace lw_client {

	enum SshVersion
	{
		ssh1 = 0,
		ssh2
	};

	class LWClient_API SshClientParam : public ClientParam
	{
	public:
		SshClientParam(const std::string& host, int port = 22);
		~SshClientParam();

	public:
		SshVersion GetSshVerion() const;
		void SetSshVersion(SshVersion ver);

		bool IsLoginByPrivateKey();
		void LoginByPrivateKey(bool bValue);

		void GetPrivateKey(std::string& strFileName, std::string& strContent) const;
		void SetPrivateKey(const std::string& strFileName, const std::string& strContent);

		std::string GetPriKeyFile() const;
		void SetPriKeyFile(const std::string& strFileName);

	protected:
		SshVersion m_sshVer;

		bool m_bLoginByPrivateKey;
		//login by private key
		std::string m_strPriKeyFile;
		std::string m_strKeyName;
		std::string m_strPrivateKey;
	};
	typedef std::shared_ptr<SshClientParam> SshConnParamPtr;

}



