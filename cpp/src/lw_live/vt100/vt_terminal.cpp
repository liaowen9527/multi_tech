#include "vt_terminal.h"
#include "terminal.h"

vt_terminal::vt_terminal()
{
	m_term = new terminal;
}

vt_terminal::~vt_terminal()
{
	delete m_term;
	m_term = nullptr;
}

void vt_terminal::set_callback(std::function<void()> func)
{
	m_callback = func;
}

void vt_terminal::parse(const char* str, int len)
{
	m_term->get_parser()->parse(str, len);
	if (m_callback)
	{
		m_callback();
	}
}

void vt_terminal::get_size(int& rows, int cols)
{
	rows = m_term->get_rows();
	cols = m_term->get_cols();
}

bool vt_terminal::is_show_cursor()
{
	return m_term->get_conf()->is_cursor_on();
}

void vt_terminal::get_cursor(int& row, int& col)
{
	screen* sc = m_term->get_screen();
	cursor curs = sc->get_cursor();
	row = curs.row;
	col = curs.col;
}

int vt_terminal::get_sblines()
{
	return m_term->get_sblines();
}

int vt_terminal::get_lines()
{
	return m_term->get_lines();
}

std::wstring vt_terminal::get_text(int line)
{
	termline_ptr tline = m_term->get_termline(line);
	if (tline)
	{
		return tline->get_string();
	}
	
	return std::wstring();
}

std::wstring vt_terminal::get_text(int from, int to)
{
	std::wstring ret;
	return ret;
}

std::wstring vt_terminal::get_all_text()
{
	std::wstring ret;
	int lines = m_term->get_lines();
	for (int i = 0; i < lines; ++i)
	{
		ret += get_text(i);
		if (i != lines - 1)
		{
			ret += L"\r\n";
		}
	}
	
	return ret;
}

int vt_terminal::get_vscrollbar()
{
	return m_term->get_ui()->get_disptop();
}

void vt_terminal::set_vscrollbar(int top)
{
	m_term->get_ui()->set_disptop(top);
}

