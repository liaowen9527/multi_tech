#include "LWLogInstance.h"
#include "filesystem/process.h"

static LWLogInstance s_inst;

LWLogInstance::LWLogInstance()
{
	m_pid = CurrentProcess::GetPid();
}

LWLogInstance::~LWLogInstance()
{
}

LWLogInstance* LWLogInstance::Instance()
{
	return &s_inst;
}

bool LWLogInstance::CanLog(const char* inst, LogLevel nLevel)
{
	auto pr = m_loggers.equal_range(inst);
	for (auto itr = pr.first; itr != pr.second; ++itr)
	{
		auto& pLogger = itr->second;
		if (pLogger->CanLog(nLevel))
		{
			return true;
		}
	}
	return false;
}

void LWLogInstance::Log(const char* inst, const char* funcName, LogLevel nLevel, const std::string& strMsg)
{
	auto pr = m_loggers.equal_range(inst);
	for (auto itr = pr.first; itr != pr.second; ++itr)
	{
		auto& pLogger = itr->second;
		if (pLogger->CanLog(nLevel))
		{
			pLogger->Log();
		}
	}
}
