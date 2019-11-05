#include "parser_vt52_y2.h"
#include "../terminal.h"


parser_vt52_y2::parser_vt52_y2()
{
	
}

parser_vt52_y2::~parser_vt52_y2()
{

}

void parser_vt52_y2::parse()
{
	get_parser()->set_termstate(TOPLEVEL);

	char ch = get_char();
	int col = ch - ' ';

	cursor curs = get_screen()->get_cursor();
	get_control()->move_cursor(curs.row, col, 0);
}
