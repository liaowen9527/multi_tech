#include "multi_thread_wrapper.h"
#ifdef WIN32
#include <windows.h>
#endif

namespace lwutil {

	multi_thread_wrapper::multi_thread_wrapper()
		: m_bQuit(false)
		, m_bPause(false)
		, m_bRecordSleepThread(false)
	{

	}

	multi_thread_wrapper::~multi_thread_wrapper()
	{
		quit(true);
	}

	bool multi_thread_wrapper::run(int nCount /* = 2 */, bool bPause/* = false*/, int nSleepBeforeRun /* = 0 */)
	{
		std::lock_guard<std::mutex> guard(m_mutex_thread);
		if (!m_vecThread.empty())
		{
			return false;
		}

		m_bQuit = false;
		m_bPause = bPause;
		m_nSleepMsecBeforeRun = nSleepBeforeRun;

		for (int i = 0; i < nCount; i++)
		{
			m_vecThread.push_back(new_thread_unsafe());
		}

		return true;
	}

	bool multi_thread_wrapper::is_running()
	{
		std::lock_guard<std::mutex> guard(m_mutex_thread);
		return !m_vecThread.empty();
	}

	void multi_thread_wrapper::quit(bool bWait)
	{
		std::lock_guard<std::mutex> guard(m_mutex_thread);
		m_bQuit = true;

		m_conditionVar.notify_all();
		
		if (bWait)
		{
			wait_quit_unsafe();
		}
	}

	void multi_thread_wrapper::wait_quit()
	{
		std::lock_guard<std::mutex> guard(m_mutex_thread);
		wait_quit_unsafe();
	}

	void multi_thread_wrapper::pause()
	{
		m_bPause = true;
		m_conditionVar.notify_one();
	}

	void multi_thread_wrapper::resume()
	{
		m_bPause = false;
		m_conditionVar.notify_one();
	}

	void multi_thread_wrapper::record_sleep_thread(bool bValue)
	{
		m_bRecordSleepThread = bValue;
	}

	void multi_thread_wrapper::set_priority(int priority)
	{
#ifdef WIN32
		std::lock_guard<std::mutex> guard(m_mutex_thread);
		for (int i = 0; i < m_vecThread.size(); ++i)
		{
			std::thread* pThread = m_vecThread[i];
			::SetThreadPriority((HANDLE)(pThread->native_handle()), priority);
		}
#endif
	}

	void multi_thread_wrapper::wait_quit_unsafe()
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

	std::thread* multi_thread_wrapper::new_thread_unsafe()
	{
		return new std::thread(&multi_thread_wrapper::inner_thread, this);
	}

	void multi_thread_wrapper::inner_thread()
	{
		int left_msec = m_nSleepMsecBeforeRun;
		while (!m_bQuit)
		{
			if (left_msec <= 0)
			{
				break;
			}

			int msec = left_msec > 1000 ? 1000 : left_msec;

			std::this_thread::sleep_for(std::chrono::milliseconds(msec));

			left_msec -= msec;
		}

		while (!m_bQuit)
		{
			if (!m_bPause)
			{
				inner_thread_func();
			}
			
			into_sleep();
			thread_sleep();
			out_sleep();
		}
	}

	void multi_thread_wrapper::thread_sleep()
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		if (m_bQuit)
		{
			return;
		}
		m_conditionVar.wait_for(lck, std::chrono::milliseconds(500));
	}

	bool multi_thread_wrapper::is_all_thread_sleep_unsafe()
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

	void multi_thread_wrapper::into_sleep()
	{
		if (m_bRecordSleepThread)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			m_setSleepThreadId.insert(std::this_thread::get_id());
		}
	}

	void multi_thread_wrapper::out_sleep()
	{
		if (m_bRecordSleepThread)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			m_setSleepThreadId.erase(std::this_thread::get_id());
		}
	}



	multi_thread_variable::multi_thread_variable()
		: m_threadFunc(nullptr)
		, m_sleepFunc(nullptr)
	{

	}

	multi_thread_variable::~multi_thread_variable()
	{

	}

	void multi_thread_variable::bind_thread_func(std::function<void()> func)
	{
		m_threadFunc = func;
	}

	void multi_thread_variable::bind_sleep_func(std::function<void(std::condition_variable &, std::mutex &)> func)
	{
		m_sleepFunc = func;
	}

	void multi_thread_variable::notify_one()
	{
		m_conditionVar.notify_one();
	}

	void multi_thread_variable::inner_thread_func()
	{
		if (m_threadFunc)
		{
			m_threadFunc();
		}
	}

	void multi_thread_variable::thread_sleep()
	{
		if (m_sleepFunc)
		{
			m_sleepFunc(m_conditionVar, m_mutex);
		}
		else
		{
			multi_thread_wrapper::thread_sleep();
		}
	}

}