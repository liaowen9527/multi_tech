#ifdef WIN32

#include "windows/DLLMainHelper.hpp"

static DLLMainHelper helper;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return helper.DllMain(hInstance, dwReason, lpReserved);
}

#endif