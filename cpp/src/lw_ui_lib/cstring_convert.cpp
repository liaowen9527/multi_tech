#include "cstring_convert.h"
#include "std/string_convert.h"

using namespace lw_util;

void CStringConverter::to_str(const CString& src, std::string& str)
{
#ifdef _UNICODE
	std::wstring temp = src.GetString();
	str = StringConverter::utf_to_utf(temp);
#else
	str = src.GetString();
#endif // _UNICODE
}

void CStringConverter::to_str(const CString& src, std::wstring& str)
{
#ifdef _UNICODE
	str = src.GetString();
#else
	std::string temp = src.GetString();
	str = StringConverter::utf_to_utf(temp);
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