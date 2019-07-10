#include "logger.h"
#include <stdarg.h>
#include <sstream>
#include "filesystem/process.h"
#include "log4cpp/Category.hh"

namespace lw_util {

	Logger* Logger::s_logger = nullptr;

	Logger::Logger()
	{
		m_pid = CurrentProcess::GetPid();
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

		return s_logger;
	}

	void Logger::log(const char* inst, const char* funcName, LogLevel nLevel, const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		int len = _vscprintf(format, argList) + 1;
		if (len == 0)
		{
			va_end(argList);
			return;
		}

		char * buffer = new char[len + 1];
		vsprintf_s(buffer, len, format, argList);
		va_end(argList);

		std::string msg(buffer, len);
		delete[] buffer;

		log(inst, funcName, nLevel, msg);

		return;
	}

	void Logger::log(const char* inst, const char* funcName, LogLevel nLevel, const std::string& str)
	{
		std::stringstream ss;
		ss << "[" << m_pid << ":" << CurrentThread::GetTid() << "] [" << funcName  << "] " << str;

		log4cpp::Category& logger = log4cpp::Category::getInstance(inst);

		switch (nLevel)
		{
		case LOG_DEBUG:
			logger.debug(ss.str());
			break;
		case LOG_INFO:
			logger.info(ss.str());
			break;
		case LOG_WARN:
			logger.warn(ss.str());
			break;
		case LOG_ERROR:
			logger.error(ss.str());
			break;
		default:
			break;
		}
	}

}
