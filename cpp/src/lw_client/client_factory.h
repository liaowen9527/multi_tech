#pragma once
#include <string>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "lw_client.h"
#include "client.h"

namespace lw_client {

	class LWClient_API ClientFactory
	{
	public:
		static bool InitEnv();
		static ClientPtr Create(ClientParam* param);

	};

}

