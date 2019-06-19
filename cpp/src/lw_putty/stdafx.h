// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef NB_LINUX

	#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
	#endif

	#ifndef WINVER                          // Specifies that the minimum required platform is Windows Vista.
	#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
	#endif

	#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
	#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
	#endif

	#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
	#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
	#endif

	#ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 7.0.
	#define _WIN32_IE 0x0700        // Change this to the appropriate value to target other versions of IE.
	#endif

	#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

	#include <afxwin.h>         // MFC core and standard components
	#include <afxext.h>         // MFC extensions

	#ifndef _AFX_NO_OLE_SUPPORT
	#include <afxole.h>         // MFC OLE classes
	#include <afxodlgs.h>       // MFC OLE dialog classes
	#include <afxdisp.h>        // MFC Automation classes
	#endif // _AFX_NO_OLE_SUPPORT

	#ifndef _AFX_NO_DB_SUPPORT
	#include <afxdb.h>			// MFC ODBC database classes
	#endif // _AFX_NO_DB_SUPPORT

	#ifndef _AFX_NO_DAO_SUPPORT
	#include <afxdao.h>			// MFC DAO database classes
	#endif // _AFX_NO_DAO_SUPPORT

	#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
	#ifndef _AFX_NO_AFXCMN_SUPPORT
	#include <afxcmn.h>			// MFC support for Windows Common Controls
	#endif // _AFX_NO_AFXCMN_SUPPORT

	#pragma warning (disable : 4290) 

	#pragma warning (disable: 4251)
	#pragma warning (disable: 4312)
	#pragma warning (disable: 4311)
	#pragma warning (disable: 4267)
	#pragma warning (disable: 4244)
#endif

