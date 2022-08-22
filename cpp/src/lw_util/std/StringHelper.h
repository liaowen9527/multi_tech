
#pragma once

#include "lw_util.h"
#include <string>

class StringHelper
{
public:
	static std::string NormalizeEOL(const std::string &text, const std::string& strEol = "\r\n");

	static void TrimLeft(std::string& str, const std::string& strTrim = " \t");
	static void TrimRight(std::string& str, const std::string& strTrim /*= " \t"*/);
	static void Trim(std::string& str, const std::string& strTrim /*= " \t"*/);

	static void ToUpper(std::string& str);
	static void ToLower(std::string& str);

};