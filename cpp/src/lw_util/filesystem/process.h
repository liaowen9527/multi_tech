#pragma once
#include <string>

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