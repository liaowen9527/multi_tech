#include "termchar_attr.h"
#include "termchar_attr_decode.h"

termchar_attr::termchar_attr(unsigned long attr /*= ATTR_DEFAULT*/)
{
	m_decode = nullptr;
#ifdef DEBUG
	m_decode = new termchar_attr_decode;
#endif
	
	m_attr = 0;
	m_truecolour.fg = m_truecolour.bg = g_optionalrgb_none;
	enable(attr, true);
}

termchar_attr::~termchar_attr()
{
	if (m_decode)
	{
		delete m_decode;
		m_decode = nullptr;
	}
}

void termchar_attr::enable(unsigned long attr, bool _enable /*= true*/)
{
	if (_enable)
	{
		m_attr |= attr;
	}
	else
	{
		m_attr &= ~attr;
	}

	if (m_decode)
	{
		m_decode->set_attr(m_attr);
	}
}

bool termchar_attr::is_enable(unsigned long attr)
{
	return m_attr & attr;
}

unsigned long termchar_attr::get_attr()
{
	return m_attr;
}

bool termchar_attr::is_narrow()
{
	return is_enable(ATTR_NARROW);
}

bool termchar_attr::is_wide()
{
	return is_enable(ATTR_WIDE);
}

bool termchar_attr::is_bold()
{
	return is_enable(ATTR_BOLD);
}

bool termchar_attr::is_under()
{
	return is_enable(ATTR_UNDER);
}

bool termchar_attr::is_reverse()
{
	return is_enable(ATTR_REVERSE);
}

bool termchar_attr::is_blink()
{
	return is_enable(ATTR_BLINK);
}

bool termchar_attr::is_fgmask()
{
	return is_enable(ATTR_FGMASK);
}

bool termchar_attr::is_bgmask()
{
	return is_enable(ATTR_BGMASK);
}

bool termchar_attr::is_colours()
{
	return is_enable(ATTR_COLOURS);
}

bool termchar_attr::is_dim()
{
	return is_enable(ATTR_DIM);
}

int termchar_attr::get_colour(bool bg)
{
	int attr = m_attr;
	int mask = bg ? ATTR_BGMASK : ATTR_FGMASK;
	int from_bit = bg ? ATTR_BGSHIFT : ATTR_FGSHIFT;
	
	return (attr & mask) >> from_bit;
}

truecolour* termchar_attr::get_truecolour()
{
	return &m_truecolour;
}

void termchar_attr::set_colour(int ncolor, bool bg)
{
	int from_bit = bg ? ATTR_BGSHIFT : ATTR_FGSHIFT;
	int colour = ncolor << from_bit;

	set_colour_attr(colour, bg);
}

void termchar_attr::set_colour_default(bool bg)
{
	int colour = bg ? ATTR_BGSHIFT : ATTR_FGSHIFT;
	set_colour_attr(colour, bg);
}

void termchar_attr::set_colour_attr(int colour, bool bg)
{
	int attr = m_attr;
	int mask = bg ? ATTR_BGMASK : ATTR_FGMASK;
	optional_rgb& op_rgb = bg ? m_truecolour.bg : m_truecolour.fg;

	//remove color
	attr &= ~mask;
	//add color
	attr |= colour;

	m_attr = attr;
	op_rgb = g_optionalrgb_none;
}

void termchar_attr::set_truecolour(int r, int g, int b, bool bg)
{
	optional_rgb& op_rgb = bg ? m_truecolour.bg : m_truecolour.fg;
	op_rgb.enabled = true;

	color_rgb& rgb = op_rgb.rgb;
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
}
