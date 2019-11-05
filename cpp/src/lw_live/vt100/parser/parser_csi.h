#pragma once
#include "parser_base.h"

class parser_csi : public parser_base
{
public:
	parser_csi();
	~parser_csi();

public:
	virtual void parse();

protected:
	void parser_digit(char ch);
	void new_arg();
	void set_esc_query(char ch);
	void esc_control();

protected:
	void parser_0(char ch);
	void parser_comb(char query, char ch);
};

