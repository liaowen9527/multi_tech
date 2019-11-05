#pragma once
#include "parser_base.h"

class parser_osc_p : public parser_base
{
public:
	parser_osc_p();
	~parser_osc_p();

public:
	virtual void parse();
};

