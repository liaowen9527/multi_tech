#include "StringConvert.h"
#include <boost/locale.hpp>

std::string StringConvert::utf_to_utf(const std::wstring& src)
{
	return boost::locale::conv::utf_to_utf<char>(src);
}

std::wstring StringConvert::utf_to_utf(const std::string& src)
{
	return boost::locale::conv::utf_to_utf<wchar_t>(src);
}

std::string StringConvert::to_utf(const std::string& src, const std::string& from_charset/* = "UTF-8"*/)
{
	return boost::locale::conv::to_utf<char>(src, from_charset);
}

std::wstring StringConvert::to_unicode(const std::string& src, const std::string& from_charset/* = "UTF-8"*/)
{
	return boost::locale::conv::to_utf<wchar_t>(src, from_charset);
}

std::string StringConvert::from_utf(const std::string& src, const std::string& to_charset/* = "UTF-8"*/)
{
	return boost::locale::conv::from_utf(src, to_charset);
}

std::string StringConvert::from_unicode(const std::wstring& src, const std::string& to_charset/* = "UTF-8"*/)
{
	return boost::locale::conv::from_utf(src, to_charset);
}

std::string StringConvert::between(const std::string& src, const std::string& from_charset, const std::string& to_charset)
{
	return boost::locale::conv::between(src, to_charset, from_charset);
}