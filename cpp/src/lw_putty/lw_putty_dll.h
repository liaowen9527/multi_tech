#pragma once

#ifdef WIN32
	#ifdef LWPutty_DLL
	#define LWPutty_API __declspec(dllexport)
	#else
	#define LWPutty_API __declspec(dllimport)
#endif
	#define LWPutty_API
#endif

