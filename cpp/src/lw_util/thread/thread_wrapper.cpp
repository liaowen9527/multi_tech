#include "stdafx.h"
#include "thread_wrapper.h"
#include <assert.h>
#include "boost/thread.hpp"
#include "filesystem/process.h"

std::mutex s_mutex;
std::map<int, std::string> s_threads;	//for view all threads and thread name

thread_wrapper::thread_wrapper()
	: m_bQuit(false)
	, m_bPause(false)
	, m_bRecordSleepThread(false)
{

}

thread_wrapper::~thread_wrapper()
{
	quit(true);
	for (auto& pr : m_tid_datas)
	{
		delete pr.second;
	}
}

bool thread_wrapper::run(const std::string& strThreadName, int nCount, bool bPause/* = false*/, int nSleepBeforeRun /* = 0 */)
{
	assert(nCount > 0 && nCount < 1000);
	std::lock_guard<std::mutex> guard(m_mutex_thread);
	if (!m_vecThread.empty())
	{
		return false;
	}

	m_strName = strThreadName;
	m_bQuit = false;
	m_bPause = bPause;
	m_nSleepMsecBeforeRun = nSleepBeforeRun;

	for (int i = 0; i < nCount; i++)
	{
		m_vecThread.push_back(new_thread_unsafe());
	}

	return true;
}

bool thread_wrapper::is_running()
{
	std::lock_guard<std::mutex> guard(m_mutex_thread);
	return !m_vecThread.empty();
}

void thread_wrapper::quit(bool bWait)
{
	m_bQuit = true;

	m_conditionVar.notify_all();

	if (bWait)
	{
		std::lock_guard<std::mutex> guard(m_mutex_thread);
		wait_quit_unsafe();
	}
}

void thread_wrapper::wait_quit()
{
	std::lock_guard<std::mutex> guard(m_mutex_thread);
	wait_quit_unsafe();
}

void thread_wrapper::pause()
{
	m_bPause = true;
	m_conditionVar.notify_one();
}

void thread_wrapper::resume()
{
	m_bPause = false;
	m_conditionVar.notify_one();
}

void thread_wrapper::record_sleep_thread(bool bValue)
{
	m_bRecordSleepThread = bValue;
}

int thread_wrapper::get_thread_count()
{
	std::lock_guard<std::mutex> guard(m_mutex_thread);
	return m_vecThread.size();
}

void thread_wrapper::append_threads(int nCount)
{
	std::lock_guard<std::mutex> guard(m_mutex_thread);
	if (m_bQuit)
	{
		return;
	}

	for (int i = 0; i < nCount; i++)
	{
		m_vecThread.push_back(new_thread_unsafe());
	}
}

void thread_wrapper::wait_quit_unsafe()
{
	for (int i = 0; i < m_vecThread.size(); ++i)
	{
		std::thread* pThread = m_vecThread[i];
		if (pThread->joinable())
		{
			pThread->join();
		}

		delete pThread;
	}

	m_vecThread.clear();
}

bool thread_wrapper::is_quit()
{
	return m_bPause;
}

std::thread* thread_wrapper::new_thread_unsafe()
{
	return new std::thread(&thread_wrapper::inner_thread, this);
}

void thread_wrapper::inner_thread()
{
	auto tid = CurrentThread::GetTid();
	{
		std::lock_guard<std::mutex> lck(s_mutex);
		s_threads.insert(std::make_pair(tid, m_strName));
	}

	int left_msec = m_nSleepMsecBeforeRun;
	while (!m_bQuit)
	{
		if (left_msec <= 0)
		{
			break;
		}

		int msec = left_msec > 1000 ? 1000 : left_msec;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(msec));

		left_msec -= msec;
	}

	while (!m_bQuit)
	{
		while (m_bPause)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			m_conditionVar.wait(lck);

			if (m_bQuit)
			{
				return;
			}
		}

		inner_thread_func();

		into_sleep();
		thread_sleep();
		out_sleep();
	}

	{
		std::lock_guard<std::mutex> lck(s_mutex);
		s_threads.erase(tid);
	}
}

void thread_wrapper::thread_sleep()
{
	std::unique_lock<std::mutex> lck(m_mutex);
	if (m_bQuit)
	{
		return;
	}
	m_conditionVar.wait_for(lck, std::chrono::milliseconds(500));
}

bool thread_wrapper::is_all_thread_sleep_unsafe()
{
	for (int i = 0; i < m_vecThread.size(); ++i)
	{
		std::thread* pThread = m_vecThread[i];
		if (m_setSleepThreadId.find(pThread->get_id()) == m_setSleepThreadId.end())
		{
			return false;
		}
	}
	return true;
}

void thread_wrapper::into_sleep()
{
	if (m_bRecordSleepThread)
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		m_setSleepThreadId.insert(std::this_thread::get_id());
	}
}

void thread_wrapper::out_sleep()
{
	if (m_bRecordSleepThread)
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		m_setSleepThreadId.erase(std::this_thread::get_id());
	}
}



thread_variable::thread_variable()
	: m_threadFunc(nullptr)
	, m_sleepFunc(nullptr)
{

}

thread_variable::~thread_variable()
{

}

void thread_variable::bind_thread_func(std::function<void()> func)
{
	m_threadFunc = func;
}

void thread_variable::bind_sleep_func(std::function<void(std::condition_variable &, std::mutex &)> func)
{
	m_sleepFunc = func;
}

void thread_variable::notify_one()
{
	m_conditionVar.notify_one();
}

void thread_variable::notify_all()
{
	m_conditionVar.notify_all();
}

std::mutex& thread_variable::get_mutex()
{
	return m_mutex;
}

void thread_variable::inner_thread_func()
{
	if (m_threadFunc)
	{
		m_threadFunc();
	}
}

void thread_variable::thread_sleep()
{
	if (m_sleepFunc)
	{
		m_sleepFunc(m_conditionVar, m_mutex);
	}
	else
	{
		thread_wrapper::thread_sleep();
	}
}


timer::timer()
{

}

timer::~timer()
{

}

void timer::add_timer(int seconds, std::function<void()> func)
{
	timer_job temp;
	temp.nInterval = seconds;
	temp.func = func;
	temp.tLastCall = time(0);

	std::unique_lock<std::mutex> lck(m_mutex);
	m_jobs.push_back(temp);
}

void timer::inner_thread_func()
{
	std::vector<timer_job> timers;
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		m_jobs.swap(timers);
	}

	for (auto& temp : timers)
	{
		time_t tNow = time(0);
		int duration = (int)(tNow - temp.tLastCall);
		if (duration > temp.nInterval)
		{
			temp.func();
			temp.tLastCall = tNow;
		}
	}

	{
		std::unique_lock<std::mutex> lck(m_mutex);
		timers.insert(timers.end(), m_jobs.begin(), m_jobs.end());
		m_jobs.swap(timers);
	}
}

void timer::thread_sleep()
{
	boost::this_thread::sleep_for(boost::chrono::seconds(1));
}