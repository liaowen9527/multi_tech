#include "live_param.h"

namespace lw_live {

	LiveParam::LiveParam()
	{
		m_bAutoLogin = false;
	}

	LiveParam::~LiveParam()
	{

	}

	lw_client::ClientParamPtr LiveParam::GetClientParam()
	{
		return m_clientParam;
	}

	void LiveParam::SetClientParam(lw_client::ClientParamPtr clientParam)
	{
		m_clientParam = clientParam;
	}

	std::string LiveParam::GetEnableUserName() const
	{
		return m_strEnableUserName;
	}

	std::string LiveParam::GetEnablePassword() const
	{
		return m_strEnablePassword;
	}

	bool LiveParam::IsAutoLogin() const
	{
		return m_bAutoLogin;
	}

}


