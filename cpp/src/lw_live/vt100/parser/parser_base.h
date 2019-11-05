#pragma once

class terminal;
class control;
class conf;
class screen;
class parser;
class argument;
class osc_string;

class parser_base
{
public:
	parser_base();
	~parser_base();

public:
	terminal* get_term();
	void set_term(terminal* term);

	control* get_control();
	parser* get_parser();
	argument* get_argument();
	osc_string* get_osc_string();
	conf* get_conf();
	screen* get_screen();
	char get_char();
	wchar_t get_wchar();

	virtual void parse();

protected:
	terminal* m_term;
};

