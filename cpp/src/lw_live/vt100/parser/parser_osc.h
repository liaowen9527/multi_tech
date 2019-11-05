#pragma once
#include "parser_base.h"

class parser_osc : public parser_base
{
public:
	parser_osc();
	~parser_osc();

public:
	virtual void parse();
};

