#pragma once
#include <string>
#include <memory>
#include "lw_live.h"
#include "client_param.h"

namespace lw_live {

	class LWLive_API Destination
	{
	public:
		Destination();
		virtual ~Destination();

	public:
		void GetClientParam() const;
		void SetClientParam(lw_client::ClientParamPtr clientParam);

		virtual std::string GetLoginUserName() const;

		virtual std::string GetLoginPassword() const;

		virtual std::string GetEnableUserName() const;

		virtual std::string GetEnablePassword() const;

		bool IsAutoLogin() const;

	protected:
		lw_client::ClientParamPtr m_clientParam;
	};

	typedef std::shared_ptr<Destination> DestinationPtr;

}


