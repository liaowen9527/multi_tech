#include "parser.h"
#include "../terminal.h"

parser::parser()
{
	m_term = nullptr;
	m_termstate = TOPLEVEL;
}

parser::~parser()
{

}

void parser::set_term(terminal* term)
{
	m_term = term;
	m_vtC0.set_term(term);
	m_vtTopLevel.set_term(term);
	m_vtEsc.set_term(term);
	m_vtCsi.set_term(term);
	m_vtOsc.set_term(term);
	m_vtOscW.set_term(term);
	m_vtOscP.set_term(term);
	m_vtOscStr.set_term(term);
	m_vtMaybeST.set_term(term);
	m_vt52Esc.set_term(term);
	m_vt52Y1.set_term(term);
	m_vt52Y2.set_term(term);
	m_vt52FG.set_term(term);
	m_vt52BG.set_term(term);
}

argument* parser::get_argument()
{
	return &m_args;
}

osc_string* parser::get_osc_string()
{
	return &m_osc_string;
}

wchar_t parser::get_char()
{
	return m_ucs.get_wchar();
}

utf8_decode parser::get_ucs()
{
	return m_ucs;
}

void parser::parse(const std::string& str)
{
	parse(str.c_str(), str.length());
}

void parser::parse(const char* str, int len)
{
	const char* str_old = str;
	int len_old = len;
	while (len)
	{
		parse_char(*str);

		str = str + 1;
		--len;
	}

}

void parser::parse_char(char ch)
{
	if (m_ucs.is_finish())
	{
		m_ucs.reset();
	}
	if (!m_ucs.append(ch) || !m_ucs.is_finish())
	{
		return;
	}

	if (is_c1_control())
	{
		c1_control();
	}

	if (is_gl_control())
	{
		gl_control();
	}
	else if(is_c0_control())
	{
		m_vtC0.parse();
	}
	else
	{
		normal_control();
	}
	
}

void parser::c1_control()
{
	wchar_t ch = get_char();
	if (ch == 0x9a)
	{
		ch = 0;
	}
	else 
	{
		set_termstate(SEEN_ESC);
		get_argument()->set_esc_query(0);
		
		//ch = '@' + (ch & 0x1F);
	}
}

void parser::gl_control()
{
	screen* sc = m_term->get_screen();
	cursor curs = sc->get_cursor();
	if (!sc->is_wrap_next())
	{
		if (curs.col)
		{
			curs.col -= 1;
		}
		sc->set_curs_col(curs.col);
	}

	sc->set_wrap_next(false);
}

void parser::normal_control()
{
	switch (get_termstate())
	{
	case TOPLEVEL:
		m_vtTopLevel.parse();
		break;
		case SEEN_ESC:
			m_vtEsc.parse();
			break;
		case SEEN_CSI:
			m_vtCsi.parse();
			break;
		case SEEN_OSC:
			m_vtOsc.parse();
			break;
		case SEEN_OSC_W:
			m_vtOscW.parse();
			break;
		case SEEN_OSC_P:
			m_vtOscP.parse();
			break;
		case OSC_STRING:
			m_vtOscStr.parse();
			break;
		case OSC_MAYBE_ST:
			m_vtMaybeST.parse();
			break;
		case VT52_ESC:
			m_vt52Esc.parse();
			break;
		case VT52_Y1:
			m_vt52Y1.parse();
			break;
		case VT52_Y2:
			m_vt52Y2.parse();
			break;
		case VT52_FG:
			m_vt52FG.parse();
			break;
		case VT52_BG:
			m_vt52BG.parse();
			break;
	default:
		break;
	}
}

bool parser::is_gl_control()
{
	wchar_t ch = get_char();
	if (ch != 127) //'\177' | 0x7F, ascii, DEL (delete)
	{
		return false;
	}

	return get_termstate() < DO_CTRLS && m_term->is_compat(OTHER);
}

bool parser::is_c0_control()
{
	wchar_t ch = get_char();
	if (ch > 31) //0x1F, ascii
	{
		return false;
	}

	return get_termstate() < DO_CTRLS;		
}

bool parser::is_c1_control()
{
	return false;
	wchar_t ch = get_char();
	char c = ch; // = ch & 0xFF
	if (c >= 0x2F) //'\177' | 0x7F, ascii, DEL (delete)
	{
		return false;
	}

	if (m_term->get_conf()->is_vt52mode())
	{
		return false;
	}

	return get_termstate() < DO_CTRLS && m_term->is_compat(VT220);
}

termstate parser::get_termstate()
{
	return m_termstate;
}

void parser::set_termstate(termstate state)
{
	m_termstate = state;
}
