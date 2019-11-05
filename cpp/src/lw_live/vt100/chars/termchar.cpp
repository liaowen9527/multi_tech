#include "termchar.h"

termchar::termchar(wchar_t ch /*= ' '*/)
{
	m_char = ch;
	m_ucs = false;
}

termchar::~termchar()
{

}

wchar_t termchar::get_char()
{
	return m_char;
}

void termchar::set_char(wchar_t ch)
{
	m_char = ch;
	set_ucswide(false);
}

termchar_attr* termchar::get_attr()
{
	return &m_attr;
}

truecolour* termchar::get_colour()
{
	return m_attr.get_truecolour();
}

bool termchar::is_wchar()
{
	return m_char > 0xFF;
}

bool termchar::is_ucswide()
{
	return m_ucs;
}

void termchar::set_ucswide(bool val)
{
	m_ucs = val;
}
