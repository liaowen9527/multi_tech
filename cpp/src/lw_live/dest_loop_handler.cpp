#include "dest_loop_handler.h"

namespace lw_live {

	DestLoopHandler::DestLoopHandler()
		: m_bTryNext(true)
	{

	}

	DestLoopHandler::~DestLoopHandler()
	{

	}

	DestinationPtr DestLoopHandler::GetNext()
	{
		if (!m_bTryNext)
		{
			return nullptr;
		}


		return nullptr;
	}

	DestinationPtr DestLoopHandler::GetCurrent()
	{

	}

	bool DestLoopHandler::CanTryNext()
	{
		return m_bTryNext;
	}

	void DestLoopHandler::SetTryNext(bool val)
	{
		m_bTryNext = val;
	}

}


