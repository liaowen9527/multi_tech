#include "destination.h"


namespace lw_live {

	LiveParamPtr Destination::GetClient()
	{
		return m_clients.GetRootItem();
	}

	LiveParamPtr Destination::GetRetryClient(LiveParamPtr client)
	{
		return m_clients.GetNextSiblingItem(client);
	}

	LiveParamPtr Destination::GetNextClient(LiveParamPtr client)
	{
		return m_clients.GetChildItem(client);
	}

}


