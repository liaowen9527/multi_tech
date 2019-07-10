#pragma once
#include <mutex>
#include <condition_variable>

class WaitEvent
{
public:
	WaitEvent();
	virtual ~WaitEvent();

public:
	void NotifyAll();
	void NotifyOne();

	//unit: second
	bool Wait(int nTimeout = 60);
	bool Wait(std::function<bool()> funcBreak, int nTimeout = 60);

protected:
	std::condition_variable m_condVar;
	std::mutex m_mutex;
};

