#pragma once
#include "parser_base.h"

class parser_osc_w : public parser_base
{
public:
	parser_osc_w();
	~parser_osc_w();

public:
	virtual void parse();
};

