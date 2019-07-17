#pragma once
#include <string>
#include <memory>
#include "lw_live.h"
#include "client_param.h"

namespace lw_live {

	class LWLive_API LiveParam
	{
	public:
		LiveParam();
		virtual ~LiveParam();

	public:
		lw_client::ClientParamPtr GetClientParam();
		void SetClientParam(lw_client::ClientParamPtr clientParam);

		std::string GetEnableUserName() const;
		std::string GetEnablePassword() const;

		bool IsAutoLogin() const;

	protected:
		lw_client::ClientParamPtr m_clientParam;

		std::string m_strEnableUserName;
		std::string m_strEnablePassword;
		bool m_bAutoLogin;
	};

	typedef std::shared_ptr<LiveParam> LiveParamPtr;

}


