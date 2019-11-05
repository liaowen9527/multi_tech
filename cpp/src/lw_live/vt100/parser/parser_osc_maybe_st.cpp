#include "parser_osc_maybe_st.h"
#include "../terminal.h"


parser_osc_maybe_st::parser_osc_maybe_st()
{
	
}

parser_osc_maybe_st::~parser_osc_maybe_st()
{

}

void parser_osc_maybe_st::parse()
{
	char ch = get_char();
	if (ch == '\\')
	{
		//do_osc(term);
		get_parser()->set_termstate(TOPLEVEL);
		return;
	}
	
	parser_esc esc;
	esc.set_term(get_term());
	esc.parse();
}
