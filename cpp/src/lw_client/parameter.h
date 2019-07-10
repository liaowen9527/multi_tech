#pragma once
#include <string>
#include "lw_client.h"
#include "defines.h"
#include <memory>

namespace lw_client {

	class LWClient_API Parameter
	{
	protected:
		Parameter(EnumConnType connType, const std::string& host, int port);
		virtual ~Parameter() = 0;

	public:
		EnumConnType GetConnType() const;
		
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
		EnumConnType m_connType;
		std::string m_host;
		int m_port;
		std::string m_username;
		std::string m_password;

		bool m_bLoginConfirm;
	};
	typedef std::shared_ptr<Parameter> ConnParamPtr;

	class LWClient_API TelnetConnParam : public Parameter
	{
	public:
		TelnetConnParam(const std::string& host, int port = 23);
		~TelnetConnParam();
	};
	typedef std::shared_ptr<TelnetConnParam> TelnetConnParamPtr;

	class LWClient_API SshConnParam : public Parameter
	{
	public:
		SshConnParam(const std::string& host, int port = 22);
		~SshConnParam();

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
	typedef std::shared_ptr<SshConnParam> SshConnParamPtr;

}



