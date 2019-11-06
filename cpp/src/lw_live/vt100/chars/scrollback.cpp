#include "scrollback.h"

scrollback::scrollback()
{
	m_max_lines = 1024;
}

scrollback::~scrollback()
{

}

int scrollback::get_rows()
{
	return m_lines.size();
}

int scrollback::get_maxrows()
{
	return m_max_lines;
}

termline_ptr scrollback::get_line(int nline)
{
	if (nline < 0 || nline >= m_lines.size())
	{
		return nullptr;
	}
	return m_lines[nline];
}

void scrollback::push_back(termline_ptr line)
{
	while (m_lines.size() >= m_max_lines)
	{
		m_lines.pop_back();
	}
	
	m_lines.push_back(line);
}

termline_ptr scrollback::pop_back()
{
	termline_ptr line = m_lines.back();
	m_lines.pop_back();

	return line;
}

void scrollback::clear()
{
	m_lines.clear();
}
