#pragma once
#include "parser_base.h"

class parser_vt52_fg : public parser_base
{
public:
	parser_vt52_fg();
	~parser_vt52_fg();

public:
	virtual void parse();
};

