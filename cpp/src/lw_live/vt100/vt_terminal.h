#pragma once
#include <string>
#include <vector>
#include <functional>
#include "lw_live.h"

class terminal;
class LWLive_API vt_terminal
{
public:
	vt_terminal();
	~vt_terminal();

	void set_callback(std::function<void()> func);

public:
	void parse(const char* str, int len);

	void get_size(int& rows, int cols);
	
	bool is_show_cursor();
	void get_cursor(int& row, int& col);

	int get_sblines();
	int get_lines();
	std::wstring get_text(int line);
	std::wstring get_text(int from, int to);
	std::wstring get_all_text();

	int get_vscrollbar();
	void set_vscrollbar(int top);

protected:
	terminal* m_term;
	std::function<void()> m_callback;
};