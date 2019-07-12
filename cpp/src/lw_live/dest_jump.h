#pragma once
#include <string>
#include <memory>
#include <vector>
#include "lw_live.h"
#include "destination.h"

namespace lw_live {

	class LWLive_API DestJump : Destination
	{
	public:
		DestJump();
		virtual ~DestJump();

	public:
		std::vector<DestinationPtr> GetDestnations();
		void SetDestnations(const std::vector<DestinationPtr>& destination);

	protected:
		std::vector<DestinationPtr> m_destination;
	};

	typedef std::shared_ptr<DestJump> DestJumpPtr;

}


