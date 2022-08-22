#pragma once
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <set>
#include <map>
#include <vector>
#include <condition_variable>

#include "lw_util.h"

class LWUtil_API thread_wrapper
{
public:
	thread_wrapper();
	virtual ~thread_wrapper();

public:
	bool run(const std::string& strThreadName, int nCount, bool bPause = false, int nSleepBeforeRun = 0);

	bool is_running();

	void quit(bool bWait);

	void wait_quit();

	void pause();
	void resume();

	void record_sleep_thread(bool bValue);

	template<class T>
	T* get_tid_data(const std::thread::id& tid)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		return get_tid_data_unsafe<T>(tid);
	}

	template<class T>
	void loop_tid_datas(std::function<bool(const std::thread::id&, T*)> func)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		loop_tid_datas_unsafe<T>(func);
	}

protected:
	int get_thread_count();
	void append_threads(int nCount);
	void wait_quit_unsafe();
	bool is_quit();

	template<class T>
	T* get_tid_data_unsafe(const std::thread::id& tid)
	{
		auto itr = m_tid_datas.find(tid);
		if (m_tid_datas.end() == itr)
		{
			T* p = new T();
			m_tid_datas[tid] = p;
			return p;
		}
		return (T*)(itr->second);
	}

	template<class T>
	void loop_tid_datas_unsafe(std::function<bool(const std::thread::id&, T*)> func)
	{
		for (auto& pr : m_tid_datas)
		{
			if (!func(pr.first, (T*)(pr.second)))
			{
				break;
			}
		}
	}

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
	std::condition_variable m_conditionVar;
	std::mutex m_mutex;

private:
	std::vector<std::thread*> m_vecThread;
	std::mutex m_mutex_thread;

	std::string m_strName;
	std::atomic_bool m_bQuit;
	std::atomic_bool m_bPause;
	int m_nSleepMsecBeforeRun;	//unit millisecond

	bool m_bRecordSleepThread;
	std::set<std::thread::id> m_setSleepThreadId;

	std::map<std::thread::id, void*> m_tid_datas;
};


class LWUtil_API thread_variable : public thread_wrapper
{
public:
	thread_variable();
	virtual ~thread_variable();

public:
	void bind_thread_func(std::function<void()> func);

	void bind_sleep_func(std::function<void(std::condition_variable &, std::mutex &)> func);

	void notify_one();
	void notify_all();
	std::mutex& get_mutex();

protected:
	virtual void inner_thread_func();
	virtual void thread_sleep();

protected:
	std::function<void()> m_threadFunc;
	std::function<void(std::condition_variable&, std::mutex&)> m_sleepFunc;
};


class LWUtil_API timer : public thread_wrapper
{
public:
	timer();
	~timer();

	struct timer_job
	{
		int nInterval;	//seconds
		time_t tLastCall;
		std::function<void()> func;
	};

public:
	void add_timer(int seconds, std::function<void()> func);

protected:
	virtual void inner_thread_func();
	virtual void thread_sleep();

protected:
	std::vector<timer_job> m_jobs;
};


