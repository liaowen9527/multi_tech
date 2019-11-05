#pragma once
#include "parser_base.h"

class parser_osc_string : public parser_base
{
public:
	parser_osc_string();
	~parser_osc_string();

public:
	virtual void parse();
};

