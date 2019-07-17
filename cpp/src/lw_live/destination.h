#pragma once
#include <string>
#include <memory>
#include "lw_live.h"
#include "live_param.h"
#include "std/tree.h"

namespace lw_live {

	class LWLive_API Destination
	{
	public:
		LiveParamPtr GetClient();
		LiveParamPtr GetRetryClient(LiveParamPtr client);
		LiveParamPtr GetNextClient(LiveParamPtr client);

	public:
		lw_util::Tree<LiveParamPtr> m_clients;
	};

	typedef std::shared_ptr<Destination> DestinationPtr;

}


