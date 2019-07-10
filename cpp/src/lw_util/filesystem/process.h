#pragma once
#include <string>

namespace lw_util {

	class Process
	{
	public:
		Process();
		~Process();

	public:
		int GetPid();
		void SetPid(int pid);

	protected:
		int m_pid;
	};

	class CurrentProcess
	{
	public:
		static int GetPid();
		static std::string GetName();
	};

	class CurrentThread
	{
	public:
		static int GetTid();
	};

}