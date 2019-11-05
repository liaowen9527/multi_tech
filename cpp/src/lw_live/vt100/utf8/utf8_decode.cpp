#include "utf8_decode.h"
#include "convert.h"

utf8_decode::utf8_decode()
	: m_size(0)
{
	
}

utf8_decode::~utf8_decode()
{

}

void utf8_decode::reset()
{
	m_utf8.clear();
	m_size = 0;
}

bool utf8_decode::append(char c)
{
	if (m_utf8.empty())
	{
		m_size = get_len_by_first_char(c);
	}
	if (m_size < 1)
	{
		return false;
	}

	m_utf8.append(1, c);

	return true;
}

bool utf8_decode::is_finish()
{
	return m_utf8.size() == m_size;
}

wchar_t utf8_decode::get_wchar()
{
	std::wstring wstr = convert::to_unicode(m_utf8);
	if (wstr.empty())
	{
		return ' ';
	}

	return wstr.at(0);
}

bool utf8_decode::is_wide()
{
	return m_size > 1;
}

int utf8_decode::get_len_by_first_char(unsigned char c)
{
	if (c < 0x80)	//[0, 128)
	{
		return 1;
	}
	if ((c & 0xe0) == 0xc0) //[0xc0, 0xe0)
	{
		return 2;
	}
	if ((c & 0xf0) == 0xe0) //[0xe0, 0xf0)
	{
		return 3;
	}
	else if ((c & 0xf8) == 0xf0) //[0xf0, 0xf8)
	{
		return 4;
	}
	else if ((c & 0xfc) == 0xf8) //[0xf8, 0xfc)
	{
		return 5;
	}
	else if ((c & 0xfe) == 0xfc) //[0xfc, 0xfe)
	{
		return 6;
	}

	return -1;
}
