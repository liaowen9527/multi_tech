#include "dest_jump.h"

namespace lw_live {

	DestJump::DestJump()
	{

	}

	DestJump::~DestJump()
	{

	}

	std::vector<DestinationPtr> DestJump::GetDestnations()
	{
		return m_destination;
	}

	void DestJump::SetDestnations(const std::vector<DestinationPtr>& destination)
	{
		m_destination = destination;
	}

}


