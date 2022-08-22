#pragma once
/*
 *  Created on: 2022/06/30
 *      Author: LIAO WEN
 */
#include "LWLog.h"
#include <map>

class LWLogInstance
{
public:
	LWLogInstance();
	~LWLogInstance();

	static LWLogInstance* Instance();

public:
	bool CanLog(const char* inst, LogLevel nLevel);

	void Log(const char* inst, const char* funcName, LogLevel nLevel, const std::string& strMsg);

protected:
	int m_pid;	//process id

	std::multimap<std::string, LWLoggerPtr> m_loggers;
};

