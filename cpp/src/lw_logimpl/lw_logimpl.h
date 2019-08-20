#pragma once

#ifdef WIN32
#ifdef LWLogImpl_DLL
#define LWLogImpl_API __declspec(dllexport)
#else
#define LWLogImpl_API __declspec(dllimport)
#endif
#else
#define LWLogImpl_API
#endif

