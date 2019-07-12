#pragma once

#ifdef WIN32
	#ifdef LWLive_DLL
		#define LWLive_API __declspec(dllexport)
	#else
		#define LWLive_API __declspec(dllimport)
	#endif
#else
	#define LWLive_API
#endif

