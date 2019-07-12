
#pragma once

#include "logger.h"


#define InstMain "main"
#define InstFilesystem "filesystem"
#define InstNet "net"
#define InstServer "server"
#define InstClient "client"
#define InstLive "live"

#ifdef Log
#undef Log
#endif

#define EasyLog(inst, level, ...) \
	lw_util::Logger::Instance()->log(inst, __FUNCTION__, level, __VA_ARGS__);
