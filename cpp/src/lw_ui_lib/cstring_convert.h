
#pragma once

#include "lw_ui_lib.h"
#include <string>

class LWUI_API CStringConverter
{
public:
	static std::string to_str(const CString& src);
	static std::wstring to_wstr(const CString& src);

	static CString to_cstr(const std::string& src);
	static CString to_cstr(const std::wstring& src);
};