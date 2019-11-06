#include "ui.h"

ui::ui()
{
	m_rows = 0;
	m_cols = 0;
	m_disptop = 0;
	m_alt_sblines = 0;
}

ui::~ui()
{

}

int ui::get_rows()
{
	return m_rows;
}

void ui::set_rows(int rows)
{
	m_rows = rows;
}

int ui::get_cols()
{
	return m_cols;
}

void ui::set_cols(int cols)
{
	m_cols = cols;
}

int ui::get_disptop()
{
	return m_disptop;
}

void ui::set_disptop(int top)
{
	m_disptop = top;
}

int ui::get_alt_sblines()
{
	return m_alt_sblines;
}

void ui::set_alt_sblines(int lines)
{
	m_alt_sblines = lines;
}
