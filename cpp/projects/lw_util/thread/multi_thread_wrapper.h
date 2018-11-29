#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <set>

#include "util_dll.h"

namespace lwutil {

	class UTIL_API multi_thread_wrapper
	{
	public:
		multi_thread_wrapper();
		virtual ~multi_thread_wrapper();

	public:
		bool run(int nCount = 2, bool bPause = false, int nSleepBeforeRun = 0);

		bool is_running();

		void quit(bool bWait);

		void wait_quit();

		void pause();
		void resume();

		void record_sleep_thread(bool bValue);

		//THREAD_PRIORITY_NORMAL, see winbase.h
		void set_priority(int priority);

	protected:
		void wait_quit_unsafe();

	protected:
		virtual std::thread* new_thread_unsafe();
		virtual void inner_thread();
		virtual void inner_thread_func() = 0;
		virtual void thread_sleep();

	protected:
		bool is_all_thread_sleep_unsafe();
		void into_sleep();
		void out_sleep();

	protected:
		std::vector<std::thread*> m_vecThread;
		std::mutex m_mutex_thread;

		std::atomic_bool m_bQuit;
		std::atomic_bool m_bPause;
		std::atomic_int m_nSleepMsecBeforeRun;	//unit millisecond

		bool m_bRecordSleepThread;
		std::set<std::thread::id> m_setSleepThreadId;

	protected:
		std::condition_variable m_conditionVar;
		std::mutex m_mutex;
	};


	class multi_thread_variable : public multi_thread_wrapper
	{
	public:
		multi_thread_variable();
		virtual ~multi_thread_variable();

	public:
		void bind_thread_func(std::function<void()> func);

		void bind_sleep_func(std::function<void(std::condition_variable &, std::mutex &)> func);

		void notify_one();

	protected:
		virtual void inner_thread_func();
		virtual void thread_sleep();

	protected:
		std::function<void()> m_threadFunc;
		std::function<void(std::condition_variable&, std::mutex&)> m_sleepFunc;
	};
}


