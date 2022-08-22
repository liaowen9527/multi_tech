#include "LWLog.h"
#include <stdarg.h>

#include "LWLogInstance.h"
#include "LWHelper.h"

void Log2(const char* inst, const char* funcName, LogLevel nLevel, const char* format, ...)
{
	auto pInst = LWLogInstance::Instance();
	if (!pInst->CanLog(inst, nLevel))
	{
		return;
	}
	
	std::string strMsg;
	lw_va_to_string(format, strMsg);

	pInst->Log(inst, funcName, nLevel, strMsg);
}

LWLogger::LWLogger()
{

}

LWLogger::~LWLogger()
{

}

bool LWLogger::CanLog(LogLevel nLevel)
{
	return false;
}
