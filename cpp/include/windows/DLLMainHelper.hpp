#include "PlatformMacros.h"

#if TARGET_PLATFORM == PLATFORM_WIN32

#include <afxdllx.h>
#include <iostream>

class DLLMainHelper
{
public:
	DLLMainHelper()
	{
		m_module = NULL;
		memset(&m_extmodule, 0, sizeof(AFX_EXTENSION_MODULE));
		m_dll = NULL;
	}
	~DLLMainHelper()
	{
		SafeFreeDynLinkLib();
	}

public:
	int DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
	{
		switch (dwReason)
		{
		case DLL_PROCESS_ATTACH:
			return (OnProcessAttach(hInstance, lpReserved));
		case DLL_THREAD_ATTACH:
			return (OnThreadAttach(hInstance, lpReserved));
		case DLL_THREAD_DETACH:
			return (OnThreadDetach(hInstance, lpReserved));
		case DLL_PROCESS_DETACH:
			return (OnProcessDetach(hInstance, lpReserved));
		}

		return 1;
	}

	HMODULE GetModule()
	{
		if (m_extmodule.hModule)
			return m_extmodule.hModule;

		return (m_module);
	}

protected:
	int OnProcessAttach(HINSTANCE hInstance, LPVOID lpReserved)
	{
		if (!AfxInitExtensionModule(m_extmodule, hInstance))
			return 0;

		SafeFreeDynLinkLib();

		m_dll = new CDynLinkLibrary(m_extmodule);
		if (m_dll == NULL)
			return 0;

		m_module = (HMODULE)hInstance;

		TCHAR lpFileName[MAX_PATH];
		::GetModuleFileName(m_module, lpFileName, MAX_PATH);
		m_strModuleName = lpFileName;

		return 1;
	}

	int OnProcessDetach(HINSTANCE hInstance, LPVOID lpReserved)
	{
		SafeFreeDynLinkLib();

		AfxTermExtensionModule(m_extmodule);

		return 1;
	}

	int OnThreadAttach(HINSTANCE hInstance, LPVOID lpReserved)
	{
		return 1;
	}

	int OnThreadDetach(HINSTANCE hInstance, LPVOID lpReserved)
	{
		return 1;
	}

protected:
	void SafeFreeDynLinkLib()
	{
		if (m_dll)
		{
			delete m_dll;
			m_dll = NULL;
		}
	}

protected:
	HMODULE m_module;
	AFX_EXTENSION_MODULE m_extmodule;
	CDynLinkLibrary* m_dll;
	CString m_strModuleName;
};



#endif