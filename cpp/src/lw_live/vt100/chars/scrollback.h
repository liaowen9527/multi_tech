#pragma once
#include <deque>
#include "termline.h"

class scrollback
{
public:
	scrollback();
	~scrollback();

public:
	void push_back(termline_ptr line);
	termline_ptr pop_back();

protected:
	std::deque<termline_ptr> m_lines;
	int m_max_lines;
};