#pragma once
#include <deque>
#include "termline.h"

class screen_lines
{
public:
	screen_lines();
	~screen_lines();

public:
	void resize(int rows, int cols);

	termline_ptr get_line(int row);
	termline_ptr front();
	termline_ptr back();

	termline_ptr pop_front();
	termline_ptr pop_back();

public:
	std::wstring get_string(std::wstring crlf = L"\r\n");
	int find_last_nonempty_line();

protected:
	std::deque<termline_ptr> m_lines;
	int m_rows;
	int m_cols;
};