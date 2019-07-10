
#pragma once

#include "lw_util.h"
#include <string>

enum LogLevel
{
	LOG_INVALID = -1,
	LOG_DEBUG = 0,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
};

namespace lw_util {

	class LWUtil_API Logger
	{
	public:
		Logger();
		~Logger();

		static Logger* Instance();

		void log(const char* inst, const char* funcName, LogLevel nLevel, const char* format, ...);

		void log(const char* inst, const char* funcName, LogLevel nLevel, const std::string& str);

	protected:
		static Logger* s_logger;
		
		int m_pid;
	};

}