#include "osc_string.h"

#define OSC_STR_MAX 2048

osc_string::osc_string()
{
	m_osc_w = false;
}

osc_string::~osc_string()
{

}

bool osc_string::is_osc_w()
{
	return m_osc_w;
}

void osc_string::set_osc_w(bool val)
{
	m_osc_w = val;
}

int osc_string::length()
{
	return m_str.length();
}

void osc_string::resize(int len)
{
	m_str.resize(len);
}

void osc_string::append(char c)
{
	m_str.append(1, c);
}
