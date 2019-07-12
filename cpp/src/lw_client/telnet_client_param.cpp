#include "telnet_client_param.h"

namespace lw_client {

	TelnetClientParam::TelnetClientParam(const std::string& host, int port /*= 23*/)
		: ClientParam(host, port)
	{

	}

	TelnetClientParam::~TelnetClientParam()
	{

	}

}






