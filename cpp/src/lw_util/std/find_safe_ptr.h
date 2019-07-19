
#pragma once

#include "lw_util.h"
#include <memory>
#include <mutex>
#include <map>

static std::mutex s_mutex;


template<class TObject>
class FindSafePtr
{
	typedef std::shared_ptr<TObject> TObjectPtr;
public:
	static std::map<TObject*, TObjectPtr> s_objs;

public:
	static TObjectPtr Find(TObject* pObj)
	{
		std::lock_guard<std::mutex> lck(s_mutex);
		auto itr = s_objs.find(pObj);
		if (s_objs.end() != itr)
		{
			return itr->second;
		}
		return nullptr;
	}

	static void Register(TObjectPtr objPtr)
	{
		if (nullptr == objPtr)
		{
			return;
		}
		std::lock_guard<std::mutex> lck(s_mutex);
		s_objs[objPtr.get()] = objPtr;
	}

	static void UnRegister(TObject* pObj)
	{
		std::lock_guard<std::mutex> lck(s_mutex);
		s_objs.erase(pObj);
	}

};

template<class TObject> 
std::map<TObject*, std::shared_ptr<TObject>> FindSafePtr<TObject>::s_objs;