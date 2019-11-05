#pragma once
#include <map>

struct color_rgb
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct optional_rgb
{
	bool enabled;
	color_rgb rgb;
};

extern const optional_rgb g_optionalrgb_none;

struct truecolour
{
	optional_rgb fg;
	optional_rgb bg;
};

class colours
{
public:
	colours();
	~colours();

public:
	color_rgb get_color(int key);

protected:
	void init_by_conf();
	void init_others();

protected:
	std::map<int, color_rgb> m_colours;
};