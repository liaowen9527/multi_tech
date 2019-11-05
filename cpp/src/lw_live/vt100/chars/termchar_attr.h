#pragma once
#include "../conf/colours.h"

#define ATTR_NARROW  0x0800000U
#define ATTR_WIDE    0x0400000U
#define ATTR_BOLD    0x0040000U
#define ATTR_UNDER   0x0080000U
#define ATTR_REVERSE 0x0100000U
#define ATTR_BLINK   0x0200000U
#define ATTR_FGMASK  0x00001FFU
#define ATTR_BGMASK  0x003FE00U
#define ATTR_COLOURS 0x003FFFFU
#define ATTR_DIM     0x1000000U
#define ATTR_FGSHIFT 0
#define ATTR_BGSHIFT 9

#define ATTR_DEFFG   (256 << ATTR_FGSHIFT)
#define ATTR_DEFBG   (258 << ATTR_BGSHIFT)
#define ATTR_DEFAULT (ATTR_DEFFG | ATTR_DEFBG)

class termchar_attr_decode;

class termchar_attr
{
public:
	termchar_attr(unsigned long attr = ATTR_DEFAULT);
	~termchar_attr();

public:
	void enable(unsigned long attr, bool _enable = true);
	bool is_enable(unsigned long attr);

	unsigned long get_attr();
	truecolour* get_truecolour();

public:
	bool is_narrow();
	bool is_wide();
	bool is_bold();
	bool is_under();
	bool is_reverse();
	bool is_blink();
	bool is_fgmask();
	bool is_bgmask();
	bool is_colours();
	bool is_dim();

	int get_colour(bool bg);
	void set_colour(int ncolor, bool bg);
	void set_colour_default(bool bg);
	void set_colour_attr(int colour, bool bg);
	void set_truecolour(int r, int g, int b, bool bg);

protected:
	unsigned long m_attr;
	termchar_attr_decode* m_decode;
	truecolour m_truecolour;
};