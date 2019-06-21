#pragma once

#ifdef WIN32
	#ifdef LWUI_DLL
	#define LWUI_API __declspec(dllexport)
	#else
	#define LWUI_API __declspec(dllimport)
#endif
	#define LWUI_API
#endif

