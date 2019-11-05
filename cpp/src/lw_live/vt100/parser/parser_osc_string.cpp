#include "parser_osc_string.h"
#include "../terminal.h"


parser_osc_string::parser_osc_string()
{
	
}

parser_osc_string::~parser_osc_string()
{

}

void parser_osc_string::parse()
{
	char ch = get_char();
	if (ch == '\012' || ch == '\015') 
	{
		get_parser()->set_termstate(TOPLEVEL);
	}
	else if (ch == 0234 || ch == '\007') 
	{
		//do_osc(term);
		get_parser()->set_termstate(TOPLEVEL);
	}
	else if (ch == '\033')
	{
		get_parser()->set_termstate(OSC_MAYBE_ST);
	}
	else
	{
		get_osc_string()->append(ch);
	}
}
