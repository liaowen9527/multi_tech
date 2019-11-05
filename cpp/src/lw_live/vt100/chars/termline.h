#pragma once
#include <vector>
#include <string>
#include "termchar.h"

class termline
{
public:
	termline(int cols);
	~termline();

public:
	void resize(int cols);

	bool is_width();

	termchar_ptr get_char(int col);

	bool is_wrapnext();
	void set_wrapnext(bool val);

	std::wstring get_string();
	bool is_empty_line();

protected:
	int find_last_not_of(wchar_t ch);

protected:
	int m_cols;
	std::vector<termchar_ptr> m_chars;
	bool m_wrapnext;
};

typedef std::shared_ptr<termline> termline_ptr;