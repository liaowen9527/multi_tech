#include "scrollback.h"

scrollback::scrollback()
{

}

scrollback::~scrollback()
{

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
