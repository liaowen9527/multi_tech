#include "stdafx.h"
#include "vtcharfilter.h"


CVtCharFilter::CVtCharFilter()
{
	m_terminal.SetCallback(this);
}

CVtCharFilter::~CVtCharFilter()
{
    
}

CVtTerminal& CVtCharFilter::GetTerminal()
{
	return m_terminal;
}

void CVtCharFilter::Parse(const char* str, int len)
{
	m_terminal.TermData(str, len);
}

void CVtCharFilter::GetClipLines(std::vector<VtLine>& lines, bool bSwap)
{
	if (bSwap)
	{
		lines.swap(m_clipLines);
	}
	else
	{
		lines = m_clipLines;
	}
}

void CVtCharFilter::ClearClipLines()
{
	m_clipLines.clear();
}

void CVtCharFilter::OnClipLine(void* termline)
{
	m_clipLines.push_back(VtLine());
	VtLine& vtLine = m_clipLines.back();

	m_terminal.GetTermLine(termline, vtLine);
}
