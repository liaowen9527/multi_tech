#include "putty_ssh.h"
#include <assert.h>
#include "putty_conf.h"

PuttySsh::PuttySsh()
{
	SetSshMode(conf);
}

PuttySsh::~PuttySsh()
{

}

bool PuttySsh::Connect(const std::string& strRemoteHost, int nPort /*= 22*/)
{
	return PuttyClient::Connect(strRemoteHost, nPort);
}

void PuttySsh::SetSshVer(EnumSshVer ver)
{
	switch (ver)
	{
	case ssh_v1:
		SetSshVer1(conf);
		break;
	case ssh_v2:
		SetSshVer2(conf);
		break;
	default:
		assert(false);
		break;
	}
}

void PuttySsh::SetKeyfile(const std::string& strKeyFile)
{
	if (!strKeyFile.empty())
	{
		SetKeyFile(conf, strKeyFile.c_str());
	}
}

int PuttySsh::from_backend(int is_stderr, const char *data, int len)
{
	if (!m_bLogin && is_stderr == 0)
	{
		m_bLogin = true;
		if (m_pNotify)
		{
			m_pNotify->OnLoginResult(true);
		}
	}
	return __super::from_backend(is_stderr, data, len);
}

