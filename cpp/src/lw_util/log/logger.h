
#pragma once

#include "lw_util.h"
#include <string>
#include <vector>
#include "log_entity.h"
#include "logger_impl.h"

namespace lw_util {

	class LWUtil_API Logger
	{
	public:
		Logger();
		~Logger();

		static Logger* Instance();

		void AddLogger(LoggerImplPtr logPtr);

		virtual bool CanLog(LogLevel nLevel);
		virtual void Log(LogEntityPtr ptr);

	protected:
		static Logger* s_logger;
		static int s_pid;

		std::vector<LoggerImplPtr> m_vecLogger;
	};
	
}