#include "log_entity.h"
#include <stdarg.h>
#include "std/time_helper.h"
#include "filesystem/process.h"

namespace lw_util {

	LogEntity::LogEntity(LogLevel emLevel/* = LOG_INFO*/)
		: m_level(emLevel)
	{
		m_local = lw_util::TimeHelper::LocalUtcTime();
		m_pid = 0;
		m_tid = lw_util::CurrentThread::GetTid();
	}

	LogEntity::~LogEntity()
	{

	}

	LogEntityPtr LogEntity::Create(const char* inst, const char* funcName, LogLevel nLevel, const char* format, ...)
	{
		if (!CanLog(inst, nLevel))
		{
			return nullptr;
		}

		LogEntityPtr ptr = std::make_shared<LogEntity>();
		ptr->m_inst = inst;
		ptr->m_function = funcName;

		va_list argList;
		va_start(argList, format);

		int len = _vscprintf(format, argList) + 1;
		if (len == 0)
		{
			va_end(argList);
			return nullptr;
		}

		char * buffer = new char[len + 1];
		vsprintf_s(buffer, len, format, argList);
		va_end(argList);

		std::string msg(buffer, len);
		delete[] buffer;

		ptr->m_message = msg;

		return ptr;
	}

	bool LogEntity::CanLog(const char* inst, LogLevel nLevel)
	{
		return false;
	}

}
