#include "colours.h"

const optional_rgb g_optionalrgb_none = { 0, 0, 0, 0 };
#define NCFG_COLOURS 22
#define NEXT_COLOURS 240

colours::colours()
{
	init_by_conf();
}

colours::~colours()
{

}

color_rgb colours::get_color(int key)
{
	auto itr = m_colours.find(key);
	if (m_colours.end() == itr)
	{
		return g_optionalrgb_none.rgb;
	}

	return itr->second;
}

void colours::init_by_conf()
{
	/*const int ww[] = 
	{
		256, 257, 258, 259, 260, 261,
		0, 8, 1, 9, 2, 10, 3, 11,
		4, 12, 5, 13, 6, 14, 7, 15
	};*/
	m_colours[0] = { 187, 187, 187 };
	m_colours[1] = { 255, 255, 255 };
	m_colours[2] = { 0, 0, 0 };
	m_colours[3] = { 85, 85, 85 };
	m_colours[4] = { 0, 0, 0 };
	m_colours[5] = { 0, 255, 0 };
	m_colours[6] = { 0, 0, 0 };
	m_colours[7] = { 85, 85, 85 };
	m_colours[8] = { 187, 0, 0 };
	m_colours[9] = { 255, 85, 85 };
	m_colours[10] = { 0, 187, 0 };
	m_colours[11] = { 85, 255, 85 };
	m_colours[12] = { 187, 187, 0 };
	m_colours[13] = { 255, 255, 85 };
	m_colours[14] = { 0, 0, 187 };
	m_colours[15] = { 85, 85, 255 };
	m_colours[16] = { 187, 0, 187 };
	m_colours[17] = { 255, 85, 255 };
	m_colours[18] = { 0, 187, 187 };
	m_colours[19] = { 85, 255, 255 };
	m_colours[20] = { 187, 187, 187 };
	m_colours[21] = { 255, 255, 255 };
}

void colours::init_others()
{
	/*for (i = 0; i < NEXT_COLOURS; i++)
	{
		if (i < 216)
		{
			int r = i / 36, g = (i / 6) % 6, b = i % 6;
			defpal[i + 16].rgbtRed = r ? r * 40 + 55 : 0;
			defpal[i + 16].rgbtGreen = g ? g * 40 + 55 : 0;
			defpal[i + 16].rgbtBlue = b ? b * 40 + 55 : 0;
		}
		else
		{
			int shade = i - 216;
			shade = shade * 10 + 8;
			defpal[i + 16].rgbtRed = defpal[i + 16].rgbtGreen =
				defpal[i + 16].rgbtBlue = shade;
		}
	}*/
}
