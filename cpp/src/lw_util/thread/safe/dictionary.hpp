#pragma once

#include <mutex>
#include <map>
#include <vector>

namespace thread_safe {
	
	template<class K, class V>
	class dictionary
	{
	public:
		bool get(const K& key, V& value)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			auto itr = m_data.find(key);
			if (m_data.end() == itr)
			{
				return false;
			}

			value = itr->second;
			return true;
		}

		bool keys(std::vector<K>& ret)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			ret.reserve(m_data.size());

			for (auto itr = m_data.begin(); itr != m_data.end(); ++itr)
			{
				ret.push_back(itr->first);
			}
		}

		void remove_if(std::function<bool(const K&, const V&)> is_match, bool onlyone = false)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			for (auto itr = m_data.begin(); itr != m_data.end(); )
			{
				if (is_match(itr->first, itr->second))
				{
					itr = m_data.erase(itr);
					if (onlyone)
					{
						break;
					}
				}
				else
				{
					itr++;
				}
			}
		}

		void for_each(std::function<void(const K&, const V&, bool&)> func)
		{
			bool next = true;

			std::lock_guard<std::mutex> guard(m_mutex);
			for (auto itr = m_data.begin(); itr != m_data.end(); ++itr)
			{
				func(itr->first, itr->second, next);
				if (!next)
				{
					break;
				}
			}
		}

		bool has_key(const K& key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			return m_data.find(key) != m_data.end();
		}

		void append(const K& key, const V& value)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_data.insert(std::make_pair(key, value));
		}

		void remove(const K& key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_data.erase(key);
		}

		bool remove(const K& key, V& value)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			auto itr = m_data.erase(key);
			if (m_data.end() == itr)
			{
				return false;
			}

			value = *itr;
			return true;
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

		void clear()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_data.clear();
		}

		void swap(std::map<K, V>& other)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_data.swap(other);
		}

		std::map<K, V> data()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			return m_data;
		}

	private:
		std::map<K, V> m_data;
		std::mutex m_mutex;
	};
}