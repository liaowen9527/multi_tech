#include "cstring_convert.h"
#include "std/string_convert.h"

using namespace lw_util;

std::string CStringConverter::to_str(const CString& src)
{
#ifdef _UNICODE
	std::wstring temp = src.GetString();
	return StringConverter::utf_to_utf(temp);
#else
	return src.GetString();
#endif // _UNICODE
}

std::wstring CStringConverter::to_wstr(const CString& src)
{
#ifdef _UNICODE
	return src.GetString();
#else
	std::string temp = src.GetString();
	return StringConverter::utf_to_utf(temp);
#endif // _UNICODE
}

CString CStringConverter::to_cstr(const std::string& src)
{
#ifdef _UNICODE
	std::wstring temp = StringConverter::utf_to_utf(src);
	return temp.c_str();
#else
	return src.c_str();
#endif // _UNICODE
}

CString CStringConverter::to_cstr(const std::wstring& src)
{
#ifdef _UNICODE
	return src.c_str();
#else
	std::string temp = StringConverter::utf_to_utf(src);
	return temp.c_str();
#endif // _UNICODE
}