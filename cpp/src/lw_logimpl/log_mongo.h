
#pragma once

#include "lw_logimpl.h"
#include <string>
#include "log/logger_impl.h"

namespace lw_util {

	class LWLogImpl_API LogMongo : public LoggerImpl
	{
	public:
		LogMongo();
		~LogMongo();

	public:
		

	protected:
		virtual void Log(LogEntityPtr ptr);
	};

}