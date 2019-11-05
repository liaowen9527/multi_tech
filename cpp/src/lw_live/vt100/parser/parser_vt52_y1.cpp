#include "parser_vt52_y1.h"
#include "../terminal.h"


parser_vt52_y1::parser_vt52_y1()
{
	
}

parser_vt52_y1::~parser_vt52_y1()
{

}

void parser_vt52_y1::parse()
{
	get_parser()->set_termstate(VT52_Y2);

	char ch = get_char();
	int row = ch - ' ';

	cursor curs = get_screen()->get_cursor();
	get_control()->move_cursor(row, curs.col, 0);
}
