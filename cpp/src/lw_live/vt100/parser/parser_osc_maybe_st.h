#pragma once
#include "parser_base.h"

class parser_osc_maybe_st : public parser_base
{
public:
	parser_osc_maybe_st();
	~parser_osc_maybe_st();

public:
	virtual void parse();

};

