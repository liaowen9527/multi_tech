
#pragma once

#include "lw_logimpl.h"
#include <string>
#include "log/logger_impl.h"

namespace lw_util {

	class LWLogImpl_API Logfile : public LoggerImpl
	{
	public:
		Logfile();
		~Logfile();

		virtual void Log(LogEntityPtr ptr);
	};

}