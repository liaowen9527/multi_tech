#pragma once

#ifdef WIN32
#ifdef LWUtil_DLL
#define LWUtil_API __declspec(dllexport)
#else
#define LWUtil_API __declspec(dllimport)
#endif
#else
#define LWUtil_API
#endif

