#include "parser_osc_w.h"
#include "../terminal.h"


parser_osc_w::parser_osc_w()
{
	
}

parser_osc_w::~parser_osc_w()
{

}

void parser_osc_w::parse()
{
	char ch = get_char();
	switch (ch)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		get_argument()->stream_in(ch);
		break;
	default:
		get_parser()->set_termstate(OSC_STRING);
		get_osc_string()->resize(0);
		break;
	}
}
