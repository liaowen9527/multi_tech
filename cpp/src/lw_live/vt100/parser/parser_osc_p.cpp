#include "parser_osc_p.h"
#include "../terminal.h"

parser_osc_p::parser_osc_p()
{
	
}

parser_osc_p::~parser_osc_p()
{

}

void parser_osc_p::parse()
{
	char ch = get_char();
	int osc_strlen = get_osc_string()->length();
	int max = (osc_strlen == 0 ? 21 : 15);
	
	int val = 0;
	if ('0' <= ch && ch <= '9')
	{
		val = ch - '0';
	}
	else if ('A' <= ch && ch <= 'A' + max - 10)
	{
		val = ch - 'A' + 10;
	}
	else if ('a' <= ch && ch <= 'a' + max - 10)
	{
		val = ch - 'a' + 10;
	}
	else 
	{
		get_parser()->set_termstate(TOPLEVEL);
		return;
	}

	osc_string* osc_str = get_osc_string();
	osc_str->append(val);
	
	if (osc_str->length() >= 7)
	{
		/*palette_set(term->frontend, term->osc_string[0],
			term->osc_string[1] * 16 + term->osc_string[2],
			term->osc_string[3] * 16 + term->osc_string[4],
			term->osc_string[5] * 16 + term->osc_string[6]);*/
		
		get_parser()->set_termstate(TOPLEVEL);
	}
}
