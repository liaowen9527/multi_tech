#pragma once

#ifdef WIN32

#ifdef LW_UTIL_DLL
#define UTIL_API __declspec(dllexport)
#else
#define UTIL_API __declspec(dllimport)
#endif

#else
	#define UTIL_API
#endif