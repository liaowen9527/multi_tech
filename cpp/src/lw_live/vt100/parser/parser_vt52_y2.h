#pragma once
#include "parser_base.h"

class parser_vt52_y2 : public parser_base
{
public:
	parser_vt52_y2();
	~parser_vt52_y2();

public:
	virtual void parse();
};

