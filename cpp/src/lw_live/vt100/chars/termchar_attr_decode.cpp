#include "termchar_attr_decode.h"
#include "termchar_attr.h"

termchar_attr_decode::termchar_attr_decode()
{
	set_attr(ATTR_DEFAULT);
	m_nfg = 0;
	m_nbg = 0;
}

termchar_attr_decode::~termchar_attr_decode()
{

}

unsigned long termchar_attr_decode::get_attr()
{
	int attr = 0;
	termchar_attr tattr(0);
	if (m_arrow) tattr.enable(ATTR_NARROW, true);
	if (m_wide) tattr.enable(ATTR_WIDE, true);
	if (m_bold) tattr.enable(ATTR_BOLD, true);
	if (m_under) tattr.enable(ATTR_UNDER, true);
	if (m_reverse) tattr.enable(ATTR_REVERSE, true);
	if (m_blink) tattr.enable(ATTR_BLINK, true);
	if (m_fgmask) tattr.enable(ATTR_FGMASK, true);
	if (m_bgmask) tattr.enable(ATTR_BGMASK, true);
	if (m_colours) tattr.enable(ATTR_COLOURS, true);
	if (m_dim) tattr.enable(ATTR_DIM, true);

	return tattr.get_attr();
}

void termchar_attr_decode::set_attr(unsigned long attr)
{
	termchar_attr tattr(attr);
	m_arrow = tattr.is_narrow();
	m_wide = tattr.is_wide();
	m_bold = tattr.is_bold();
	m_under = tattr.is_under();
	m_reverse = tattr.is_reverse();
	m_blink = tattr.is_blink();
	m_fgmask = tattr.is_fgmask();
	m_bgmask = tattr.is_bgmask();
	m_colours = tattr.is_colours();
	m_dim = tattr.is_dim();
}

bool termchar_attr_decode::is_narrow()
{
	return m_arrow;
}

void termchar_attr_decode::enable_narrow(bool val)
{
	m_arrow = val;
}

bool termchar_attr_decode::is_wide()
{
	return m_wide;
}

void termchar_attr_decode::enable_wide(bool val)
{
	m_wide = val;
}

bool termchar_attr_decode::is_bold()
{
	return m_bold;
}

void termchar_attr_decode::enable_bold(bool val)
{
	m_bold = val;
}

bool termchar_attr_decode::is_under()
{
	return m_under;
}

void termchar_attr_decode::enable_under(bool val)
{
	m_under = val;
}

bool termchar_attr_decode::is_reverse()
{
	return m_reverse;
}

void termchar_attr_decode::enable_reverse(bool val)
{
	m_reverse = val;
}

bool termchar_attr_decode::is_blink()
{
	return m_blink;
}

void termchar_attr_decode::enable_blink(bool val)
{
	m_blink = val;
}

bool termchar_attr_decode::is_fgmask()
{
	return m_fgmask;
}

void termchar_attr_decode::enable_fgmask(bool val)
{
	m_fgmask = val;
}

bool termchar_attr_decode::is_bgmask()
{
	return m_bgmask;
}

void termchar_attr_decode::enable_bgmask(bool val)
{
	m_bgmask = val;
}

bool termchar_attr_decode::is_colours()
{
	return m_colours;
}

void termchar_attr_decode::enable_colours(bool val)
{
	m_colours = val;
}

bool termchar_attr_decode::is_dim()
{
	return m_dim;
}

void termchar_attr_decode::enable_dim(bool val)
{
	m_dim = val;
}
