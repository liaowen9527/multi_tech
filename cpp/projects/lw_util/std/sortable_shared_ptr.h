#pragma once
#include <memory>

/*
summary: the class is useless, because if you want to find quickly, but you must new a object first. I do not think it is a good idea.
		 Based on the reason, I think use the map is better than this.
template T must override the method operator [==, <]
*/
template<class T>
class sortable_shared_ptr
{
public:
	sortable_shared_ptr() : m_sharedPtr(nullptr) {}
	sortable_shared_ptr(std::shared_ptr<T>& ptr) : m_sharedPtr(ptr) {}
	sortable_shared_ptr(const T& temp) : m_sharedPtr(std::make_shared<T>(temp)) {}
	sortable_shared_ptr(const sortable_shared_ptr& _Right) : m_sharedPtr(_Right.m_sharedPtr) {}
	~sortable_shared_ptr() { m_sharedPtr = nullptr; }

public:
	sortable_shared_ptr& operator=(const sortable_shared_ptr& _Right)
	{
		m_sharedPtr = _Right.m_sharedPtr;
	}
	operator bool() const
	{
		return (m_sharedPtr.get() != nullptr);
	}
	bool operator == (const sortable_shared_ptr& _Right) const
	{
		if (nullptr == _Right.m_sharedPtr || nullptr == m_sharedPtr) { return m_sharedPtr == _Right.m_sharedPtr; }

		return *m_sharedPtr == *_Right.m_sharedPtr;
	}
	bool operator == (T* _Right) const
	{
		return m_sharedPtr.get() == _Right;
	}
	bool operator < (const sortable_shared_ptr& _Right) const
	{
		if (nullptr == _Right.m_sharedPtr || nullptr == m_sharedPtr) { return m_sharedPtr < _Right.m_sharedPtr; }

		return *m_sharedPtr < *_Right.m_sharedPtr;
	}
	T* operator->()
	{
		return m_sharedPtr.get();
	}
	T& operator*() 
	{ 
		return *m_sharedPtr; 
	}

protected:
	std::shared_ptr<T> m_sharedPtr;
};