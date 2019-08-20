
#pragma once

#include "logger.h"


#define InstMain "main"
#define InstFilesystem "filesystem"
#define InstNet "net"
#define InstServer "server"
#define InstClient "client"
#define InstLive "live"


#define EasyLog(inst, level, ...) \
	lw_util::Logger::Instance()->Log(GetLogEntity(inst, level, __VA_ARGS__))

#define GetLogEntity(inst, level, ...) \
	lw_util::LogEntity::Create(inst, __FUNCTION__, level, __VA_ARGS__)
