
#pragma once

#include "lw_ui_lib.h"
#include <string>

class LWUI_API CStringConverter
{
public:
	static void to_str(const CString& src, std::string& str);
	static void to_str(const CString& src, std::wstring& str);

	static CString to_cstr(const std::string& src);
	static CString to_cstr(const std::wstring& src);
};