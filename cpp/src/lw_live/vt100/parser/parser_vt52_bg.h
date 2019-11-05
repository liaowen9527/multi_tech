#pragma once
#include "parser_base.h"

class parser_vt52_bg : public parser_base
{
public:
	parser_vt52_bg();
	~parser_vt52_bg();

public:
	virtual void parse();
};

