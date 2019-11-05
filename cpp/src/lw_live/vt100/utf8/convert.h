
#pragma once

#include <string>

class convert
{
public:
	static std::string utf_to_utf(const std::wstring& src);
	static std::wstring utf_to_utf(const std::string& src);

	static std::string to_utf(const std::string& src, const std::string& from_charset = "UTF-8");
	static std::wstring to_unicode(const std::string& src, const std::string& from_charset = "UTF-8");

	static std::string from_utf(const std::string& src, const std::string& to_charset = "UTF-8");
	static std::string from_unicode(const std::wstring& src, const std::string& to_charset = "UTF-8");

	static std::string between(const std::string& src, const std::string& from_charset, const std::string& to_charset);
};