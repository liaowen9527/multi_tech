#include "PlatformMacros.h"

#if TARGET_PLATFORM == PLATFORM_WIN32

#ifdef _AFXDLL
#include "DLLMainHelper.hpp"

static DLLMainHelper helper;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	return helper.DllMain(hInstance, dwReason, lpReserved);
}

#endif


#endif