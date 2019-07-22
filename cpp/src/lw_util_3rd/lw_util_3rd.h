#pragma once

#ifdef WIN32
#ifdef LWUtil3rd_DLL
#define LWUtil3rd_API __declspec(dllexport)
#else
#define LWUtil3rd_API __declspec(dllimport)
#endif
#else
#define LWUtil3rd_API
#endif

