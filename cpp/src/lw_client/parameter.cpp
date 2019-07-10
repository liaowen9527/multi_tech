#include "parameter.h"
#include <iostream>

namespace lw_client {

	Parameter::Parameter(EnumConnType connType, const std::string& host, int port)
		: m_connType(connType)
		, m_host(host)
		, m_port(port)
		, m_bLoginConfirm(true)
	{

	}

	Parameter::~Parameter()
	{

	}

	EnumConnType Parameter::GetConnType() const
	{
		return m_connType;
	}

	std::string Parameter::GetHost() const
	{
		return m_host;
	}

	void Parameter::SetHost(const std::string& str)
	{
		m_host = str;
	}

	int Parameter::GetPort() const
	{
		return m_port;
	}

	void Parameter::SetPort(int port)
	{
		m_port = port;
	}

	std::string Parameter::GetUsername() const
	{
		return m_username;
	}

	void Parameter::SetUsername(const std::string& str)
	{
		m_username = str;
	}

	std::string Parameter::GetPassword() const
	{
		return m_password;
	}

	void Parameter::SetPassword(const std::string& str)
	{
		m_password = str;
	}

	bool Parameter::IsConfirmLogin()
	{
		return m_bLoginConfirm;
	}

	void Parameter::SetLoginConfirm(bool bConfirm)
	{
		m_bLoginConfirm = bConfirm;
	}

	TelnetConnParam::TelnetConnParam(const std::string& host, int port /*= 23*/)
		: Parameter(emTelnet, host, port)
	{

	}

	TelnetConnParam::~TelnetConnParam()
	{

	}

	SshConnParam::SshConnParam(const std::string& host, int port /*= 22*/)
		: Parameter(emSSH, host, port)
		, m_sshVer(ssh1)
        , m_bLoginByPrivateKey(false)
	{

	}

	SshConnParam::~SshConnParam()
	{

	}

	SshVersion SshConnParam::GetSshVerion() const
	{
		return m_sshVer;
	}

	void SshConnParam::SetSshVersion(SshVersion ver)
	{
		m_sshVer = ver;
	}

	bool SshConnParam::IsLoginByPrivateKey()
	{
		return m_bLoginByPrivateKey;
	}

	void SshConnParam::LoginByPrivateKey(bool bValue)
	{
		m_bLoginByPrivateKey = bValue;
	}

	void SshConnParam::GetPrivateKey(std::string& strFileName, std::string& strContent) const
	{
		strFileName = m_strKeyName;
		strContent = m_strPrivateKey;
	}

	void SshConnParam::SetPrivateKey(const std::string& strFileName, const std::string& strContent)
	{
		m_strKeyName = strFileName;
		m_strPrivateKey = strContent;
	}

	std::string SshConnParam::GetPriKeyFile() const
	{
		return m_strPriKeyFile;
	}

	void SshConnParam::SetPriKeyFile(const std::string& strFileName)
	{
		m_strPriKeyFile = strFileName;
	}

}






