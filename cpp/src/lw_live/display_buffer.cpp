#include "display_buffer.h"
#include <algorithm>

DisplayBuffer::DisplayBuffer()
	: m_nIndexRow(0)
	, m_nIndexCol(0)
	, m_nLogicCol(0)
	, m_nOffsetLine(0)
{
	EnsureRow_unsafe(m_nIndexRow);
}

DisplayBuffer::~DisplayBuffer()
{
	
}

void DisplayBuffer::SyncReWriteLines(int nRowFrom, const std::vector<VtLine>& lines)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	EnsureRow_unsafe(nRowFrom);

	int nIndex = nRowFrom;
	for (int i = 0; i < lines.size(); ++i)
	{
		const VtLine& vtLine = lines[i];
		SetLine_unsafe(nIndex, vtLine);

		nIndex++;
	}

	while (m_buffer.size() > nIndex)
	{
		m_modify.insert(m_buffer.size() - 1);
		m_buffer.pop_back();
	}
	//assert(m_data.back() == buffer.back());
}

void DisplayBuffer::SyncSeekP(size_t nRow, size_t nCol)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	
	m_nIndexRow = nRow;
	m_nLogicCol = nCol;
	m_nIndexCol = GetRealCol_unsafe(nRow, nCol);

	EnsureRowCol_unsafe(m_nIndexRow, m_nIndexCol);
}

void DisplayBuffer::GetCursorPos(size_t& nRow, size_t& nCol)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	nRow = m_nIndexRow;
	nCol = m_nIndexCol;
}

void DisplayBuffer::GetEndPos(size_t& nRow, size_t& nCol)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	
	if (m_buffer.empty())
	{
		nRow = 0;
		nCol = 0;
	}
	else
	{
		nRow = m_buffer.size() - 1;

		VtLine& vtLine = m_buffer.back();
		std::wstring& str = vtLine.str;
		nCol = str.length();
	}
}

void DisplayBuffer::Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::vector<std::wstring>& vecStr)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	VtSelection selection;
	selection.m_from = VtPosition(nRowFrom, nColFrom);
	selection.m_to = VtPosition(nRowTo, nColTo);
	selection.m_strEof = m_strEol;

	selection.GetString(m_buffer, vecStr);
}

void DisplayBuffer::Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::wstring& str)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	VtSelection selection;
	selection.m_from = VtPosition(nRowFrom, nColFrom);
	selection.m_to = VtPosition(nRowTo, nColTo);
	selection.m_strEof = m_strEol;

	selection.GetString(m_buffer, str);
}

void DisplayBuffer::GetModifyRows(std::vector<int>& vecRow)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	vecRow.insert(vecRow.end(), m_modify.begin(), m_modify.end());
}

void DisplayBuffer::SetModify(int nRow, bool bValue)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	SetModify_unsafe(nRow, bValue);
}

void DisplayBuffer::ClearModify()
{
	std::lock_guard<std::mutex> lck(m_mutex);
	m_modify.clear();
}

void DisplayBuffer::ReadLines(std::wstring& strLines, bool bCheckCrLf/* = true*/)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	VtLines lines(0, m_buffer.size() - 1);
	lines.m_bCheckCrLf = false;
	lines.GetString(m_buffer, strLines);
}

void DisplayBuffer::ReadLines(size_t nRowFrom, size_t nRowTo, std::wstring& strLines, bool bCheckCrLf/* = true*/)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	VtLines lines(nRowFrom, nRowTo);
	lines.m_bCheckCrLf = false;
	lines.GetString(m_buffer, strLines);
}

void DisplayBuffer::LimitLines(int nLines, std::vector<VtLine>& vecPop)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	while (m_buffer.size() > nLines)
	{
		vecPop.push_back(m_buffer.front());
		m_buffer.pop_front();
		m_nOffsetLine++;
	}
}

int DisplayBuffer::LimitLines(int nLines, std::wstring& strLines)
{
	std::vector<VtLine> vecLine;
	LimitLines(nLines, vecLine);

	if (!vecLine.empty())
	{
		VtLines vtLines;
		vtLines.m_from = 0;
		vtLines.m_to = vecLine.size() - 1;

		vtLines.GetString(vecLine, strLines);

		const VtLine& endline = vecLine.back();
		if (endline.hasCrLf)
		{
			strLines.append(m_strEol);
		}
	}

	return vecLine.size();
}

void DisplayBuffer::EnsureRow_unsafe(int nRow)
{
	if (m_buffer.size() > nRow)
	{
		return;
	}

	VtLine vtLine;
	if (m_nWidth > 0)
	{
		vtLine.str.reserve(m_nWidth);
	}

	for (int i = m_buffer.size(); i <= nRow; ++i)
	{
		SetModify_unsafe(i);
		m_buffer.push_back(vtLine);
	}
}

void DisplayBuffer::EnsureRowCol_unsafe(int nRow, int nCol)
{
	EnsureRow_unsafe(nRow);

	VtLine& vtLine = m_buffer.at(nRow);
	std::wstring& str = vtLine.str;

	if (str.length() < nCol)
	{
		int nCount = nCol - str.length();
		str.append(std::wstring(nCount, ' '));

		SetModify_unsafe(nRow, true);
	}
}

void DisplayBuffer::SetLine_unsafe(int nLine, const VtLine& vtLine)
{
	if (nLine >= m_buffer.size())
	{
		m_buffer.push_back(vtLine);
		SetModify_unsafe(nLine);
	}
	else
	{
		VtLine& oldLine = m_buffer.at(nLine);
		if (oldLine.hasCrLf != vtLine.hasCrLf || oldLine.str != vtLine.str)
		{
			m_buffer.at(nLine) = vtLine;
			SetModify_unsafe(nLine);
		}
	}
}

void DisplayBuffer::SetModify_unsafe(int nRow, bool bValue/* = true*/)
{
	if (bValue)
	{
		m_modify.insert(nRow);
	}
	else
	{
		m_modify.erase(nRow);
	}
}

int DisplayBuffer::GetRealCol_unsafe(int nRow, int nLogicCol)
{
	if (m_buffer.size() <= nRow)
	{
		return nLogicCol;
	}

	VtLine& vtLine = m_buffer.at(nRow);
	return Col2Index(vtLine, nLogicCol);
}

int DisplayBuffer::GetRealIndex(int nIndex)
{
	return nIndex - m_nOffsetLine;
}

