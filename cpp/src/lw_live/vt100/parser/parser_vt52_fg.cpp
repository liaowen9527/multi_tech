#include "parser_vt52_fg.h"
#include "../terminal.h"


parser_vt52_fg::parser_vt52_fg()
{
	
}

parser_vt52_fg::~parser_vt52_fg()
{

}

void parser_vt52_fg::parse()
{
	get_parser()->set_termstate(TOPLEVEL);

}
