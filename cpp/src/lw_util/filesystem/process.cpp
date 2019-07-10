#include "process.h"

#if TARGET_PLATFORM == PLATFORM_WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#elif TARGET_PLATFORM == PLATFORM_LINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#endif

namespace lw_util {

	Process::Process()
	{

	}

	Process::~Process()
	{

	}

	int Process::GetPid()
	{
		return m_pid;
	}

	void Process::SetPid(int pid)
	{
		m_pid = pid;
	}

	int CurrentProcess::GetPid()
	{
#if TARGET_PLATFORM == PLATFORM_WIN32
		return GetCurrentProcessId();
#else
		return getpid();
#endif
	}

	std::string CurrentProcess::GetName()
	{
		char szPath[MAX_PATH + 1] = { 0 };
#if TARGET_PLATFORM == PLATFORM_WIN32
		GetModuleFileName(NULL, szPath, MAX_PATH);
#else
		char procPath[50] = { 0 };

		sprintf(procPath, "/proc/%d/exe", getpid());
		readlink(procPath, szPath, MAX_PATH);
#endif
		std::string strPath = std::string(szPath);

		std::string::size_type pos = std::string(strPath).find_last_of("\\/");

		return strPath.substr(pos + 1);
	}


	int CurrentThread::GetTid()
	{
#if TARGET_PLATFORM == PLATFORM_WIN32
		return ::GetCurrentThreadId();
#elif TARGET_PLATFORM == PLATFORM_LINUX
	#ifdef SYS_gettid
		return syscall(SYS_gettid);
	#else
		#error "SYS_gettid unavailable on this system"
	#endif

#endif
	}

	}


