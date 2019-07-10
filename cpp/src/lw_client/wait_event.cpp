#include "wait_event.h"
#include <time.h>

WaitEvent::WaitEvent()
{

}

WaitEvent::~WaitEvent()
{

}

void WaitEvent::NotifyAll()
{
	m_condVar.notify_all();
}

void WaitEvent::NotifyOne()
{
	m_condVar.notify_one();
}

bool WaitEvent::Wait(int nTimeout/* = 60*/)
{
	return Wait(nullptr, nTimeout);
}

bool WaitEvent::Wait(std::function<bool()> funcBreak, int nTimeout /*= 60*/)
{
	bool bCheckTimeout = nTimeout > 0;
	int nTimeoutTemp = nTimeout;

	time_t t1 = time(nullptr);

	while (true)
	{
		bool bTimeout = false;
		if (bCheckTimeout)
		{
			time_t t2 = time(nullptr);
			nTimeoutTemp = t2 - t1;
			if (nTimeoutTemp < 0)
			{
				return false;
			}

			std::unique_lock<std::mutex> lck(m_mutex);
			m_condVar.wait_for(lck, std::chrono::seconds(nTimeoutTemp));
		}
		else
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			m_condVar.wait(lck);
		}

		std::unique_lock<std::mutex> lck(m_mutex);
		if (funcBreak && funcBreak())
		{
			return true;
		}
	}

	return true;
}
