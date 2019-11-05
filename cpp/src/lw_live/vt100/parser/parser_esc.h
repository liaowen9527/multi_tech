#pragma once
#include "parser_base.h"

class parser_esc : public parser_base
{
public:
	parser_esc();
	~parser_esc();

public:
	virtual void parse();

protected:
	void parser_0(char ch);
	void parser_comb(char query, char ch);

protected:
	bool check_char();
};

