#pragma once

#include "PlatformMacros.h"

#if TARGET_PLATFORM == PLATFORM_WIN32
	#ifdef LWPutty_DLL
		#define LWPutty_API __declspec(dllexport)
	#else
		#define LWPutty_API __declspec(dllimport)
	#endif
#else
	#define LWPutty_API
#endif

