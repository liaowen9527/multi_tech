#include "screen_lines.h"

screen_lines::screen_lines()
{
	m_rows = 0;
	m_cols = 0;
}

screen_lines::~screen_lines()
{

}

void screen_lines::resize(int rows, int cols)
{
	int old_rows = m_rows;

	m_rows = rows;
	m_cols = cols;

	while (m_lines.size() > rows)
	{
		m_lines.pop_back();
	}
	for (int i = m_lines.size(); i < rows; ++i)
	{
		termline_ptr newline = std::make_shared<termline>(cols);
		m_lines.push_back(newline);
	}
}

int screen_lines::get_rows()
{
	return m_lines.size();
}

termline_ptr screen_lines::get_line(int row)
{
	if (row >= m_lines.size())
	{
		return nullptr;
	}
	return m_lines.at(row);
}

termline_ptr screen_lines::front()
{
	return m_lines.front();
}

termline_ptr screen_lines::back()
{
	return m_lines.back();
}

termline_ptr screen_lines::pop_front()
{
	termline_ptr line = m_lines.front();
	m_lines.pop_front();

	termline_ptr newline = std::make_shared<termline>(m_cols);
	m_lines.push_back(newline);

	return line;
}

termline_ptr screen_lines::pop_back()
{
	termline_ptr line = m_lines.back();
	m_lines.pop_back();

	termline_ptr newline = std::make_shared<termline>(m_cols);
	m_lines.push_front(newline);

	return line;
}

std::wstring screen_lines::get_string(std::wstring crlf /*= L"\r\n"*/)
{
	std::wstring ret;

	int end = find_last_nonempty_line();
	for (int i = 0; i <= end; ++i)
	{
		termline_ptr line = get_line(i);
		std::wstring temp = line->get_string();
		ret.append(temp);
		if (!line->is_wrapnext() && i != end)
		{
			ret.append(crlf);
		}
	}

	return ret;
}

int screen_lines::find_last_nonempty_line()
{
	for (int i = m_rows - 1; i >= 0; --i)
	{
		termline_ptr line = get_line(i);
		if (!line->is_empty_line())
		{
			return i;
		}
	}

	return -1;
}
