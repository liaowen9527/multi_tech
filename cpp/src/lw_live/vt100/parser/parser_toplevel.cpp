#include "parser_toplevel.h"
#include "../terminal.h"

parser_toplevel::parser_toplevel()
{
	
}

parser_toplevel::~parser_toplevel()
{

}

void parser_toplevel::parse()
{
	utf8_decode ucs = m_term->get_parser()->get_ucs();

	termchar chr(ucs.get_wchar());
	get_control()->display_graphic_char(&chr);

	m_term->set_last_graphic_char(&chr);
}

