
#pragma once

#include "lw_util.h"
#include <string>
#include <memory>

typedef enum {
	LOG_INVALID = -1,
	LOG_DEBUG = 0,		//Debug-level messages
	LOG_INFO,			//Informational
	LOG_NOTICE,			//Normal but significant condition
	LOG_WARN,			//Warning conditions
	LOG_ERROR,			//Error conditions
	LOG_CRIT,			//Critical Conditions
	LOG_ALERT,			//Action must be taken immediately
	LOG_EMERG			//Emergencies - system is unusable
} LogLevel;

namespace lw_util {

	class LogEntity;
	typedef std::shared_ptr<LogEntity> LogEntityPtr;

	class LWUtil_API LogEntity
	{
	public:
		LogEntity(LogLevel emLevel = LOG_INFO);
		virtual ~LogEntity();

		static LogEntityPtr Create(const char* inst, const char* funcName, LogLevel nLevel, const char* format, ...);

	protected:
		static bool CanLog(const char* inst, LogLevel nLevel);

	public:
		int m_pid;	//process id
		int m_tid;	//thread id
		
		LogLevel m_level;	//log level
		time_t m_local;	//local utc time

		std::string m_inst;	//log instance
		std::string m_keywords;	//key words for search
		std::string m_function;	//function name
		std::string m_message;	//log message
	};

}