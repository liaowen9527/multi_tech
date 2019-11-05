#include "termline.h"

static wchar_t s_erase_char = ' ';

termline::termline(int cols)
{
	m_cols = 0;
	m_wrapnext = false;

	resize(cols);
}

termline::~termline()
{

}

void termline::resize(int cols)
{
	int old_cols = m_cols;
	for (int i = old_cols; i < cols; ++i)
	{
		termchar_ptr chr = std::make_shared<termchar>(s_erase_char);
		m_chars.push_back(chr);
	}
	m_cols = cols;
}

bool termline::is_width()
{
	return false;
}

termchar_ptr termline::get_char(int col)
{
	return m_chars[col];
}

bool termline::is_wrapnext()
{
	return m_wrapnext;
}

void termline::set_wrapnext(bool val)
{
	m_wrapnext = val;
}

std::wstring termline::get_string()
{
	int end = find_last_not_of(s_erase_char);
	std::wstring ret;
	for (int i = 0; i <= end; ++i)
	{
		termchar_ptr chr = get_char(i);
		if (!chr->is_ucswide())
		{
			ret.append(1, chr->get_char());
		}
	}

	return ret;
}

bool termline::is_empty_line()
{
	return find_last_not_of(s_erase_char) < 0;
}

int termline::find_last_not_of(wchar_t ch)
{
	for (int i = m_cols - 1; i >= 0; --i)
	{
		termchar_ptr chr = get_char(i);
		if (chr->is_ucswide() || chr->get_char() != ch)
		{
			return i;
		}
	}

	return -1;
}
