// lw_ui_lib.h : main header file for the lw_ui_lib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "PlatformMacros.h"

#if TARGET_PLATFORM == PLATFORM_WIN32
	#ifdef LWUI_DLL
		#define LWUI_API __declspec(dllexport)
	#else
		#define LWUI_API __declspec(dllimport)
	#endif
#else
	#define LWUI_API
#endif


// ClwuilibApp
// See lw_ui_lib.cpp for the implementation of this class
//

class ClwuilibApp : public CWinApp
{
public:
	ClwuilibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
