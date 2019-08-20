
#pragma once

#include "lw_util.h"
#include "log_entity.h"

namespace lw_util {

	class LWUtil_API LoggerImpl
	{
	public:
		virtual ~LoggerImpl() {}

		virtual bool CanLog(LogLevel nLevel) { return false; }
		virtual void Log(LogEntityPtr ptr) = 0;
	};
	typedef std::shared_ptr<LoggerImpl> LoggerImplPtr;
	
}