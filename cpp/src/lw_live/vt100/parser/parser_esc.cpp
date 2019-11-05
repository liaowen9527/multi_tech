#include "parser_esc.h"
#include "../terminal.h"

parser_esc::parser_esc()
{
	
}

parser_esc::~parser_esc()
{

}

void parser_esc::parse()
{
	wchar_t ch = get_wchar();

	if (' ' <= ch && ch <= '/')
	{
		argument* arg = m_term->get_argument();
		char query = arg->get_esc_query();
		query = query ? -1 : ch;
		
		arg->set_esc_query(query);
		return;
	}

	m_term->get_parser()->set_termstate(TOPLEVEL);

	char query = m_term->get_argument()->get_esc_query();
	switch (query)
	{
	case 0:
		parser_0(ch);
		break;
	default:
		parser_comb(query, ch);
		break;
	}
}

void parser_esc::parser_0(char ch)
{
	switch (ch)
	{
	case '[': /* enter CSI mode */
		get_control()->into_csi();
		break;
	case ']': /* OSC: xterm escape sequences */
		get_control()->into_osc();
		break;
	case '7':		/* DECSC: save cursor */
		if (m_term->check_compat(VT100))
		{
			get_control()->save_cursor();
		}
		break;
	case '8':	 	/* DECRC: restore cursor */
		if (m_term->check_compat(VT100))
		{
			get_control()->save_cursor(false);
		}
		break;
	case '=':		/* DECKPAM: Keypad application mode */
		//KeypadAppMode();
		break;
	case '>':		/* DECKPNM: Keypad numeric mode */
		//KeypadNumMode();
		break;
	case 'D':	       /* IND: exactly equivalent to LF */
		if (m_term->check_compat(VT100))
		{
			m_term->nextline();
		}
		break;
	case 'E':	       /* NEL: exactly equivalent to CR-LF */
		if (m_term->check_compat(VT100))
		{
			m_term->get_screen()->set_curs_col(0);
			m_term->nextline();
		}
		break;
	case 'M':	       /* RI: reverse index - backwards LF */
		if (m_term->check_compat(VT100))
		{
			m_term->preline();
		}
		break;
	case 'Z':	       /* DECID: terminal type query */
		//TypeQuery();
		break;
	case 'c':	       /* RIS: restore power-on settings */
		//RestorePowerOn();
		break;
	case 'H':	       /* HTS: set a tab */
		if (m_term->check_compat(VT100))
		{
			cursor curs = m_term->get_screen()->get_cursor();
			m_term->set_a_tab(curs.col, true);
		}
		break;
	default:
		break;
	}
}

void parser_esc::parser_comb(char query, char ch)
{
	if (query == '#')
	{
		switch (ch)
		{
		case '8':	/* DECALN: fills screen with Es :-) */
			//FillScreen();
			break;
		case '3':	/* DECDHL: 2*height, top */
			//DoubleHeightLine();
			break;
		case '4':	/* DECDHL: 2*height, bottom */
			//DoubleHeightLine(LATTR_TOP);	
			break;
		case '5':	/* DECSWL: normal */
			//DoubleHeightLine(LATTR_NORM);
			break;
		case '6':	/* DECDWL: 2*width */
			//DoubleHeightLine(LATTR_WIDE);
			break;
		default:
			break;
		}
	}
	else if (query == '(')
	{
		switch (ch)
		{
			/* GZD4: G0 designate 94-set */
		case 'A':
			//Charset(CSET_GBCHR, VT100);
			break;
		case 'B':
			//Charset(CSET_ASCII, VT100);
			break;
		case '0':
			//Charset(CSET_LINEDRW, VT100);
			break;
			/* G1D4: G1-designate 94-set */
		case 'U':
			//Charset(CSET_SCOACS, OTHER);
			break;
		default:
			break;
		}
	}
	else if (query == ')')
	{
		switch (ch)
		{
			/* GZD4: G0 designate 94-set */
		case 'A':
			//Charset(CSET_GBCHR, VT100);
			break;
		case 'B':
			//Charset(CSET_ASCII, VT100);
			break;
		case '0':
			//Charset(CSET_LINEDRW, VT100);
			break;
			/* G1D4: G1-designate 94-set */
		case 'U':
			//Charset(CSET_SCOACS, OTHER);
			break;
		default:
			break;
		}
	}
	else if (query == '%')
	{
		switch (ch)
		{
			/* DOCS: Designate other coding system */
		case '8':	/* Old Linux code */
		case 'G':
			if (m_term->check_compat(OTHER))
			{
				m_term->support_utf(true);
			}
			break;
		case '@':
			if (m_term->check_compat(OTHER))
			{
				m_term->support_utf(false);
			}
			break;
		default:
			break;
		}
	}
	
}

bool parser_esc::check_char()
{
	return true;
}
