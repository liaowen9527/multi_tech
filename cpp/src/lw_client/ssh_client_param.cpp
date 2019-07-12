#include "ssh_client_param.h"

namespace lw_client {

	SshClientParam::SshClientParam(const std::string& host, int port /*= 22*/)
		: ClientParam(host, port)
		, m_sshVer(ssh1)
        , m_bLoginByPrivateKey(false)
	{

	}

	SshClientParam::~SshClientParam()
	{

	}

	SshVersion SshClientParam::GetSshVerion() const
	{
		return m_sshVer;
	}

	void SshClientParam::SetSshVersion(SshVersion ver)
	{
		m_sshVer = ver;
	}

	bool SshClientParam::IsLoginByPrivateKey()
	{
		return m_bLoginByPrivateKey;
	}

	void SshClientParam::LoginByPrivateKey(bool bValue)
	{
		m_bLoginByPrivateKey = bValue;
	}

	void SshClientParam::GetPrivateKey(std::string& strFileName, std::string& strContent) const
	{
		strFileName = m_strKeyName;
		strContent = m_strPrivateKey;
	}

	void SshClientParam::SetPrivateKey(const std::string& strFileName, const std::string& strContent)
	{
		m_strKeyName = strFileName;
		m_strPrivateKey = strContent;
	}

	std::string SshClientParam::GetPriKeyFile() const
	{
		return m_strPriKeyFile;
	}

	void SshClientParam::SetPriKeyFile(const std::string& strFileName)
	{
		m_strPriKeyFile = strFileName;
	}

}






