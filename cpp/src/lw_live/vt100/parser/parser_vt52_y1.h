#pragma once
#include "parser_base.h"

class parser_vt52_y1 : public parser_base
{
public:
	parser_vt52_y1();
	~parser_vt52_y1();

public:
	virtual void parse();
};

