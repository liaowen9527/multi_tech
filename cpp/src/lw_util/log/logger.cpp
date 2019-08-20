#include "logger.h"
#include <stdarg.h>
#include <sstream>
#include "filesystem/process.h"

namespace lw_util {

	Logger* Logger::s_logger = nullptr;
	int Logger::s_pid = 0;

	Logger::Logger()
	{
		
	}

	Logger::~Logger()
	{

	}

	Logger* Logger::Instance()
	{
		if (nullptr == s_logger)
		{
			s_logger = new Logger;
		}

		s_pid = lw_util::CurrentProcess::GetPid();
		return s_logger;
	}

	void Logger::AddLogger(LoggerImplPtr logPtr)
	{
		if (nullptr == logPtr)
		{
			return;
		}
		m_vecLogger.push_back(logPtr);
	}

	bool Logger::CanLog(LogLevel nLevel)
	{
		for (LoggerImplPtr logPtr : m_vecLogger)
		{
			if (logPtr->CanLog(nLevel))
			{
				return true;
			}
		}

		return false;
	}

	void Logger::Log(LogEntityPtr ptr)
	{
		if (nullptr == ptr)
		{
			return;
		}
		if (ptr->m_pid == 0)
		{
			ptr->m_pid = s_pid;
		}

		for (LoggerImplPtr logPtr : m_vecLogger)
		{
			logPtr->Log(ptr);
		}
	}
}
