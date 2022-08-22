#include "StringHelper.h"

std::string StringHelper::NormalizeEOL(const std::string &text, const std::string& strEol /*= "\r\n"*/)
{
	std::string normalized;
	normalized.reserve(text.length());

	const char *begin = text.c_str();
	const char *end = begin + text.length();
	const char *current = begin;
	while (current != end)
	{
		char c = *current++;
		if (c == '\r') // mac or dos EOL
		{
			if (*current == '\n') // convert dos EOL
				++current;
			normalized += strEol;
		}
		else if (c == '\n') // handle unix EOL & other char
		{
			normalized += strEol;
		}
		else
		{
			normalized += c;
		}
	}

	return normalized;
}

void StringHelper::TrimLeft(std::string& str, const std::string& strTrim /*= " \t"*/)
{
	str.erase(0, str.find_first_not_of(strTrim));
}

void StringHelper::TrimRight(std::string& str, const std::string& strTrim /*= " \t"*/)
{
	str.erase(str.find_last_not_of(strTrim), std::string::npos);
}

void StringHelper::Trim(std::string& str, const std::string& strTrim /*= " \t"*/)
{
	TrimLeft(str, strTrim);
	TrimRight(str, strTrim);
}

void StringHelper::ToUpper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void StringHelper::ToLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

