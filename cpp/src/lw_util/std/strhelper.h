
#pragma once

#include "lw_util.h"
#include <string>

template<typename CH>
class StrHelper
{
	typedef std::basic_string<CH> tstring;
public:
	static tstring NormalizeEOL(const tstring &text, const tstring& strEol = "\r\n")
	{
		tstring normalized;
		normalized.reserve(text.length());

		const CH *begin = text.c_str();
		const CH *end = begin + text.length();
		const CH *current = begin;
		while (current != end)
		{
			CH c = *current++;
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

	static void TrimLeft(tstring& str, const tstring& strTrim = " \t")
	{
		str.erase(0, str.find_first_not_of(strTrim));
	}

	static void TrimRight(tstring& str, size_t nIndexEnd = tstring::npos)
	{
		int i = 0;
		while (!str.empty() && str.back() == ' ')
		{
			if (nIndexEnd != tstring::npos)
			{
				if (str.size() == nIndexEnd)
				{
					break;
				}
			}

			str.pop_back();
		}
	}

	static void ToUpper(tstring& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	}

	static tstring ToUpper(const tstring& str)
	{
		tstring rtn = str;
		ToUpper(rtn);

		return rtn;
	}

	static void ToLower(tstring& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}

	static tstring ToLower(const tstring& str)
	{
		tstring rtn = str;
		ToLower(rtn);

		return rtn;
	}

	static std::string ToString(const std::wstring& wstr)
	{

	}

};