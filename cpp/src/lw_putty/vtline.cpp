#include "stdafx.h"
#include "vtline.h"
#include <algorithm>

VtLine::VtLine() : hasCrLf(false)
{

}

void VtLine::SubStr(const std::wstring& str, std::wstring& strSubStr, size_t beg, size_t end /*= std::wstring::npos*/)
{
	if (beg >= str.length())
	{
		strSubStr.clear();
		return;
	}

	size_t begPos = std::max<size_t>(0, beg);
	size_t endPos = std::min<size_t>(str.length(), end);
	strSubStr = str.substr(begPos, endPos);
}

VtPosition::VtPosition(int row /*= 0*/, int col /*= 0*/)
	: m_row(row)
	, m_col(col)
{

}

VtSelection::VtSelection()
	: m_strEof(L"\r\n")
	, m_bCheckCrLf(true)
{

}

template<class T>
void VtSelectionToString(const T& lines, const VtSelection& selection, std::wstring& str)
{
	if (lines.empty())
	{
		return;
	}

	const VtPosition& from = selection.m_from;
	const VtPosition& to = selection.m_to;
	int nLastLine = std::min<int>(to.m_row, (int)(lines.size()) - 1);

	for (int i = from.m_row; i <= nLastLine; i++)
	{
		const VtLine& vtLine = lines.at(i);
		const std::wstring& strLine = vtLine.str;

		std::wstring strTemp;
		if (i == from.m_row)
		{
			VtLine::SubStr(strLine, strTemp, from.m_col);
			str.append(strTemp);
		}
		else if (i == to.m_row)
		{
			VtLine::SubStr(strLine, strTemp, 0, to.m_col);
			str.append(strTemp);
		}
		else
		{
			str.append(strLine);
		}

		if (i != nLastLine)
		{
			if (!selection.m_bCheckCrLf || vtLine.hasCrLf)
			{
				str.append(selection.m_strEof);
			}
		}
	}
}

void VtSelection::GetString(const std::vector<VtLine>& lines, std::wstring& str)
{
	VtSelectionToString(lines, *this, str);
}

void VtSelection::GetString(const std::deque<VtLine>& lines, std::wstring& str)
{
	VtSelectionToString(lines, *this, str);
}


void VtSelection::GetString(const std::deque<VtLine>& lines, std::vector<std::wstring>& vecStr)
{
	if (lines.empty())
	{
		return;
	}

	const VtPosition& from = m_from;
	const VtPosition& to = m_to;
	int nLastLine = std::min<int>(to.m_row, (int)(lines.size()) - 1);

	std::wstring strFullLine;
	for (int i = from.m_row; i <= nLastLine; i++)
	{
		const VtLine& vtLine = lines.at(i);
		const std::wstring& strLine = vtLine.str;

		std::wstring strTemp;
		if (i == from.m_row)
		{
			VtLine::SubStr(strLine, strTemp, from.m_col);
			strFullLine.append(strTemp);
		}
		else if (i == to.m_row)
		{
			VtLine::SubStr(strLine, strTemp, 0, to.m_col);
			strFullLine.append(strTemp);
		}
		else
		{
			strFullLine.append(strLine);
		}

		if (vtLine.hasCrLf || i == nLastLine)
		{
			vecStr.push_back(std::wstring());
			strFullLine.swap(vecStr.back());
		}
	}
}

VtLines::VtLines(int from /*= 0*/, int to /*= 0*/)
	: m_strEof(L"\r\n")
	, m_from(from)
	, m_to(to)
	, m_bCheckCrLf(true)
{

}

template<class T>
void VtLinesToString(const T& lines, const VtLines& vtLines, std::wstring& strLines)
{
	if (lines.empty())
	{
		return;
	}
	int nLastLine = std::min<int>(vtLines.m_to, (int)(lines.size()) - 1);
	for (int i = vtLines.m_from; i <= nLastLine; i++)
	{
		const VtLine& vtLine = lines.at(i);
		const std::wstring& str = vtLine.str;

		strLines.append(str);

		if (i != lines.size() - 1)
		{
			if (!vtLines.m_bCheckCrLf || vtLine.hasCrLf)
			{
				strLines.append(vtLines.m_strEof);
			}
		}
	}
}

void VtLines::GetString(const std::vector<VtLine>& lines, std::wstring& str)
{
	VtLinesToString(lines, *this, str);
}

void VtLines::GetString(const std::deque<VtLine>& lines, std::wstring& str)
{
	VtLinesToString(lines, *this, str);
}
