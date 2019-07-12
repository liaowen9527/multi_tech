#pragma once
#include <string>
#include <memory>
#include "lw_client.h"
#include "client_param.h"

namespace lw_client {

	class LWClient_API TelnetClientParam : public ClientParam
	{
	public:
		TelnetClientParam(const std::string& host, int port = 23);
		~TelnetClientParam();
	};
	typedef std::shared_ptr<TelnetClientParam> TelnetClientParamPtr;

}



