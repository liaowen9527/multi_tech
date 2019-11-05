#pragma once
#include <memory>
#include "termchar_attr.h"

class termchar
{
public:
	termchar(wchar_t ch = ' ');
	~termchar();

public:
	wchar_t get_char();
	void set_char(wchar_t ch);

	termchar_attr* get_attr();
	truecolour* get_colour();
	bool is_wchar();

	bool is_ucswide();
	void set_ucswide(bool val);

protected:
	wchar_t m_char;
	bool m_ucs;
	termchar_attr m_attr;
};

typedef std::shared_ptr<termchar> termchar_ptr;