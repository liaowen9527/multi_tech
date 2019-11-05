#include "parser.h"
#include "../terminal.h"
#include "parser_base.h"

parser_base::parser_base()
{
	m_term = nullptr;
}

parser_base::~parser_base()
{

}

terminal* parser_base::get_term()
{
	return m_term;
}

void parser_base::set_term(terminal* term)
{
	m_term = term;
}

control* parser_base::get_control()
{
	return m_term->get_control();
}

parser* parser_base::get_parser()
{
	return m_term->get_parser();
}

argument* parser_base::get_argument()
{
	return m_term->get_argument();
}

osc_string* parser_base::get_osc_string()
{
	return get_parser()->get_osc_string();
}

conf* parser_base::get_conf()
{
	return m_term->get_conf();
}

screen* parser_base::get_screen()
{
	return m_term->get_screen();
}

char parser_base::get_char()
{
	parser* ps = m_term->get_parser();
	return (char)ps->get_char();
}

wchar_t parser_base::get_wchar()
{
	parser* ps = m_term->get_parser();
	return ps->get_char();
}

void parser_base::parse()
{

}
