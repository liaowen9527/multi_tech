#pragma once
#include <string>
#include <vector>
#include <deque>
#include "lw_putty_dll.h"

class CVtCallback;

class LWPutty_API VtLine
{
public:
	VtLine();

	static void SubStr(const std::wstring& str, std::wstring& strSubStr, size_t beg, size_t end = std::wstring::npos);

public:
	std::wstring str;
	bool hasCrLf;
	bool hasWchar;
};

class LWPutty_API VtPosition
{
public:
	VtPosition(int row = 0, int col = 0);

	int m_row;
	int m_col;
};

class LWPutty_API VtSelection
{
public:
	VtSelection();

	void GetString(const std::vector<VtLine>& lines, std::wstring& str);
	void GetString(const std::deque<VtLine>& lines, std::wstring& str);

	void GetString(const std::deque<VtLine>& lines, std::vector<std::wstring>& vecStr);

public:
	VtPosition m_from;
	VtPosition m_to;
	bool m_bCheckCrLf;
	std::wstring m_strEof;
};

class LWPutty_API VtLines
{
public:
	VtLines(int from = 0, int to = 0);

	void GetString(const std::vector<VtLine>& lines, std::wstring& str);
	void GetString(const std::deque<VtLine>& lines, std::wstring& str);

public:
	int m_from;
	int m_to;
	bool m_bCheckCrLf;
	std::wstring m_strEof;
};
