#pragma once

#ifdef WIN32
	#ifdef LWClient_DLL
		#define LWClient_API __declspec(dllexport)
	#else
		#define LWClient_API __declspec(dllimport)
	#endif
#else
	#define LWClient_API
#endif

