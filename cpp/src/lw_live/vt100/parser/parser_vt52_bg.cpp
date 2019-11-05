#include "parser_vt52_bg.h"
#include "../terminal.h"


parser_vt52_bg::parser_vt52_bg()
{
	
}

parser_vt52_bg::~parser_vt52_bg()
{

}

void parser_vt52_bg::parse()
{
	get_parser()->set_termstate(TOPLEVEL);
}
