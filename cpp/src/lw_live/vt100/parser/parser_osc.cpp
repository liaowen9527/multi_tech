#include "parser_osc.h"
#include "../terminal.h"


parser_osc::parser_osc()
{
	
}

parser_osc::~parser_osc()
{

}

void parser_osc::parse()
{
	get_osc_string()->set_osc_w(false);
	argument* arg_list = m_term->get_argument();
	//term->osc_w = false;
	char ch = get_char();
	switch (ch) {
	case 'P':	       /* Linux palette sequence */
		get_parser()->set_termstate(SEEN_OSC_P);
		get_osc_string()->resize(0);
		break;
	case 'R':	       /* Linux palette reset */
		//ResetPalette();
		get_parser()->set_termstate(TOPLEVEL);
		break;
	case 'W':	       /* word-set */
		get_parser()->set_termstate(SEEN_OSC_W);
		get_osc_string()->set_osc_w(true);
		break;
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
	{
		arg_list->stream_in(ch);
		break;
	}
	default:
	{
		int nargs = arg_list->count();
		int arg = arg_list->get_arg(0);
		if (ch == 'L' && arg == 2)
		{
			arg_list->set_arg(0, 1);
		}
		else if (ch == ';' && nargs == 1 && arg == 4)
		{
			arg_list->set_arg(nargs, 0);
		}
		else 
		{
			get_parser()->set_termstate(OSC_STRING);
			get_osc_string()->resize(0);
		}
		break;
	}
	}
}
