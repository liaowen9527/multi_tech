/**
* brief: a thread safe queue
* author: liaowen
*/

#pragma once

#include <queue>
#include <mutex>

namespace thread_safe {
	
	template<class T>
	class queue
	{
	public:
		bool front(T& ret)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			if (m_data.empty())
			{
				return false;
			}

			ret = m_data.front();
			return true;
		}

		void push(const T& val)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_data.push(val);
		}

		void pop()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			return m_data.pop();
		}

		std::size_t size()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			return m_data.size();
		}

		bool empty()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			return m_data.empty();
		}

		void swap(std::queue<T>& other)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_data.swap(other);
		}

		std::queue<T> data()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			return m_data;
		}

	private:
		std::queue<T> m_data;
		std::mutex m_mutex;
	};
}