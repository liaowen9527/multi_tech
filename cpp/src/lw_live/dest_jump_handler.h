#pragma once
#include <string>
#include <memory>
#include "lw_live.h"
#include "dest_jump.h"

namespace lw_live {

	class LWLive_API DestJumpHandler
	{
	public:
		DestJumpHandler();
		virtual ~DestJumpHandler();

	public:
		

	protected:
		DestJumpPtr m_destination;
	};



}


