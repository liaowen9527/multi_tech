#pragma once
#include <string>
#include "parser_c0.h"
#include "parser_csi.h"
#include "parser_esc.h"
#include "parser_osc.h"
#include "parser_osc_maybe_st.h"
#include "parser_osc_p.h"
#include "parser_osc_string.h"
#include "parser_osc_w.h"
#include "parser_toplevel.h"
#include "parser_vt52_bg.h"
#include "parser_vt52_esc.h"
#include "parser_vt52_fg.h"
#include "parser_vt52_y1.h"
#include "parser_vt52_y2.h"
#include "argument.h"
#include "termstate.h"
#include "osc_string.h"
#include "../utf8/utf8_decode.h"

class parser
{
public:
	parser();
	~parser();

public:
	void set_term(terminal* term);

	void parse(const std::string& str);
	void parse(const char* str, int len);

public:
	argument* get_argument();
	osc_string* get_osc_string();
	wchar_t get_char();
	utf8_decode get_ucs();

	termstate get_termstate();
	void set_termstate(termstate state);

protected:
	void parse_char(char ch);
	
	void c1_control();
	void gl_control();
	void normal_control();

protected:
	bool is_gl_control();
	bool is_c0_control();
	bool is_c1_control();

protected:
	terminal* m_term;
	termstate m_termstate;

protected:
	argument m_args;
	utf8_decode m_ucs;	//unicode string
	osc_string m_osc_string;

protected:
	parser_c0 m_vtC0;
	parser_toplevel m_vtTopLevel;
	parser_esc m_vtEsc;
	parser_csi m_vtCsi;
	parser_osc m_vtOsc;
	parser_osc_w m_vtOscW;
	parser_osc_p m_vtOscP;
	parser_osc_string m_vtOscStr;
	parser_osc_maybe_st m_vtMaybeST;
	parser_vt52_esc m_vt52Esc;
	parser_vt52_y1 m_vt52Y1;
	parser_vt52_y2 m_vt52Y2;
	parser_vt52_fg m_vt52FG;
	parser_vt52_bg m_vt52BG;
};

