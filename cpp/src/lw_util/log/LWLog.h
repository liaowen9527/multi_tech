#pragma once
/*
 *
 *  Created on: 2022/06/30
 *      Author: LIAO WEN
 */

#include <memory>
#include "lw_util.h"

#define InstMain "main"
#define InstFilesystem "filesystem"
#define InstNet "net"
#define InstServer "server"
#define InstClient "client"
#define InstLive "live"
#define InstMongo "mongo"

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


extern void Log2(const char* inst, const char* funcName, LogLevel nLevel, const char* format, ...);

#define LW_LOG(inst, level, ...) \
	Log2(inst, __FUNCTION__, level, __VA_ARGS__)


class LWUtil_API LWLogger
{
public:
	LWLogger();
	virtual ~LWLogger();

public:
	virtual bool CanLog(LogLevel nLevel);
	virtual void Log() = 0;

};
typedef std::shared_ptr<LWLogger> LWLoggerPtr;
