#include "client_factory.h"
#include <assert.h>
#include <sstream>
#include "telnet_client.h"
#include "telnet_client_param.h"
#include "ssh_client.h"
#include "ssh_client_param.h"

namespace lw_client {

	bool ClientFactory::InitEnv()
	{
		return PuttyClient::InitEnv();
	}

	ClientPtr ClientFactory::Create(ClientParam* param)
	{
		ClientPtr clientPtr = nullptr;
		if (dynamic_cast<TelnetClientParam*>(param))
		{
			clientPtr = std::make_shared<TelnetClient>();
		}
		else if (dynamic_cast<SshClientParam*>(param))
		{
			clientPtr = std::make_shared<SshClient>();
		}
		else
		{
			assert(false && "undefined connection type");
			return nullptr;
		}

		return clientPtr;
	}

	std::string ClientFactory::GetConnectString(ClientParam* param)
	{
		std::stringstream ss;
		if (dynamic_cast<TelnetClientParam*>(param))
		{
			ss << "telnet " << param->GetHost();
		}
		else if (dynamic_cast<SshClientParam*>(param))
		{
			ss << "ssh " << param->GetHost();
		}
		else
		{
			assert(false && "undefined connection type");
		}

		return ss.str();
	}

}