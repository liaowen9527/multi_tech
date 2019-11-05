#pragma once
#include "parser_base.h"

class parser_vt52_esc : public parser_base
{
public:
	parser_vt52_esc();
	~parser_vt52_esc();

public:
	virtual void parse();

};

