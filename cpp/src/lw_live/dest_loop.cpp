#include "dest_loop.h"

namespace lw_live {

	DestLoop::DestLoop()
	{

	}

	DestLoop::~DestLoop()
	{

	}

	std::vector<DestinationPtr> DestLoop::GetDestnations()
	{
		return m_destination;
	}

	void DestLoop::SetDestnations(const std::vector<DestinationPtr>& destination)
	{
		m_destination = destination;
	}

}


