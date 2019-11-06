#pragma once
#include <deque>
#include "termline.h"

class scrollback
{
public:
	scrollback();
	~scrollback();

public:
	int get_rows();
	int get_maxrows();
	termline_ptr get_line(int nline);

	void push_back(termline_ptr line);
	termline_ptr pop_back();
	void clear();

protected:
	std::deque<termline_ptr> m_lines;
	int m_max_lines;
};