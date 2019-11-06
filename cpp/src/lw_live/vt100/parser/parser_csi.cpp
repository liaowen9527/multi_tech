#include "parser_csi.h"
#include "../terminal.h"

parser_csi::parser_csi()
{
	
}

parser_csi::~parser_csi()
{

}

void parser_csi::parse()
{
	m_term->get_parser()->set_termstate(TOPLEVEL);
	char ch = get_char();

	if (isdigit(ch)) 
	{
		parser_digit(ch);
	}
	else if (ch == ';') 
	{
		new_arg();
	}
	else if (ch < '@') 
	{
		set_esc_query(ch);
	}
	else
	{
		esc_control();
	}
}

void parser_csi::parser_digit(char ch)
{
	argument* arg_list = m_term->get_parser()->get_argument();
	int nargs = arg_list->count();
	if (nargs <= ARGS_MAX)
	{
		arg_list->stream_in(ch);
	}

	m_term->get_parser()->set_termstate(SEEN_CSI);
}

void parser_csi::new_arg()
{
	argument* arg_list = m_term->get_parser()->get_argument();

	int nargs = arg_list->count();
	if (nargs < ARGS_MAX)
	{
		arg_list->set_arg(nargs, ARG_DEFAULT);
	}

	m_term->get_parser()->set_termstate(SEEN_CSI);
}

void parser_csi::set_esc_query(char ch)
{
	argument* arg_list = m_term->get_parser()->get_argument();
	char query = arg_list->get_esc_query();
	if (query)
	{
		query = -1;
	}
	else if (ch == '?')
	{
		query = 1;
	}
	else
	{
		query = ch;
	}

	arg_list->set_esc_query(query);
	m_term->get_parser()->set_termstate(SEEN_CSI);
}

void parser_csi::esc_control()
{
	char query = m_term->get_argument()->get_esc_query();
	wchar_t ch = get_wchar();

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

void parser_csi::parser_0(char ch)
{
	argument* arg_list = m_term->get_argument();
	cursor old_curs = m_term->get_screen()->get_cursor();
	switch (ch)
	{
	case 'A':       /* CUU: move up N lines */
	{
		int lines = arg_list->get_arg(0, m_term->get_rows(), 1);
		get_control()->move_cursor(old_curs.row - lines, old_curs.col, 1);
		break;
	}
	case 'e':		/* VPR: move down N lines */
		if (!m_term->check_compat(ANSI))
		{
			break;
		}
		/* FALLTHROUGH */
	case 'B':		/* CUD: Cursor down */
	{
		int lines = arg_list->get_arg(0, m_term->get_rows(), 1);
		get_control()->move_cursor(old_curs.row + lines, old_curs.col, 1);
		break;
	}
	case 'b':        /* REP: repeat previous grap */
	{
		termchar_ptr chr = m_term->get_last_graphic_char();
		if (chr)
		{
			int max_count = m_term->get_rows() * m_term->get_cols();
			int count = arg_list->get_clamp_arg(0, max_count);
			control* ctl = get_control();
			for (int i = 0; i < count; i++)
			{
				ctl->display_graphic_char(chr.get());
			}
		}
		break;
	}
	case 'a':		/* HPR: move right N cols */
		if (!m_term->check_compat(ANSI))
		{
			break;
		}
		/* FALLTHROUGH */
	case 'C':		/* CUF: Cursor right */
	{
		int cols = arg_list->get_arg(0, m_term->get_cols(), 1);
		get_control()->move_cursor(old_curs.row, old_curs.col + cols, 1);
		break;
	}
	case 'D':       /* CUB: move left N cols */
	{
		int cols = arg_list->get_arg(0, m_term->get_cols(), 1);
		get_control()->move_cursor(old_curs.row, old_curs.col - cols, 1);
		break;
	}
	case 'E':       /* CNL: move down N lines and CR */
		if (m_term->check_compat(ANSI))
		{
			int rows = arg_list->get_arg(0, m_term->get_rows(), 1);
			get_control()->move_cursor(old_curs.row + rows, 0, 1);
		}
		break;
	case 'F':       /* CPL: move up N lines and CR */
		if (m_term->check_compat(ANSI))
		{
			int rows = arg_list->get_arg(0, m_term->get_rows(), 1);
			get_control()->move_cursor(old_curs.row - rows, 0, 1);
		}
		break;
	case 'G':	      /* CHA */
	case '`':       /* HPA: set horizontal posn */
		if (m_term->check_compat(ANSI))
		{
			int col = arg_list->get_arg(0, m_term->get_cols(), 1) - 1;
			get_control()->move_cursor(old_curs.row, col, 0);
		}
		break;
	case 'd':       /* VPA: set vertical posn */
		if (m_term->check_compat(ANSI))
		{
			int row = arg_list->get_arg(0, m_term->get_rows(), 1) - 1;
			int col = old_curs.col;
			
			bool dec_om = get_screen()->is_dec_om();
			int top = dec_om ? get_screen()->get_margin().top : 0;
			int marg_clip = dec_om ? 2 : 0;
			get_control()->move_cursor(top + row, col, marg_clip);
		}
		break;
	case 'H':	     /* CUP */
	case 'f':      /* HVP: set horz and vert posns at once */
	{
		int row = arg_list->get_arg(0, m_term->get_rows(), 1) - 1;
		int col = arg_list->get_arg(1, m_term->get_cols(), 1) - 1;

		bool dec_om = get_screen()->is_dec_om();
		int top = dec_om ? get_screen()->get_margin().top : 0;
		int marg_clip = dec_om ? 2 : 0;
		get_control()->move_cursor(top + row, col, marg_clip);
		break;
	}
	case 'J':       /* ED: erase screen or parts of it */
	{
		int arg = arg_list->get_arg(0, 1);
		if (arg == 3)
		{
		}
		else
		{
			arg = arg > 2 ? 0 : arg + 1;
			bool from_beg = !!(arg & 2);
			bool to_end = !!(arg & 1);
			get_control()->erase_lots(false, from_beg, to_end);
		}
		if (get_conf()->is_scroll_on_disp())
		{
			m_term->get_ui()->set_disptop(0);
		}
		break;
	}
	case 'K':       /* EL: erase line or parts of it */
	{
		int arg = arg_list->get_arg(0, 0);
		arg = arg > 2 ? 0 : arg + 1;
		bool from_beg = !!(arg & 2);
		bool to_end = !!(arg & 1);
		get_control()->erase_lots(true, from_beg, to_end);
		break;
	}
	case 'L':       /* IL: insert lines */
		if (m_term->check_compat(VT102))
		{
			int nlines = arg_list->get_arg(0, m_term->get_rows(), 1);
			m_term->inset_lines(nlines);
		}
		break;
	case 'M':       /* DL: delete lines */
		if (m_term->check_compat(VT102))
		{
			int nlines = arg_list->get_arg(0, m_term->get_rows(), 1);
			m_term->delete_lines(nlines);
		}
		break;
	case '@':       /* ICH: insert chars */
  /* XXX VTTEST says this is vt220, vt510 manual says vt102 */
		if (m_term->check_compat(VT102))
		{
			int n = arg_list->get_arg(0, m_term->get_cols(), 1);
			m_term->insch(n);
		}
		break;
	case 'P':       /* DCH: delete chars */
		if (m_term->check_compat(VT102))
		{
			int n = arg_list->get_arg(0, m_term->get_cols(), 1);
			m_term->insch(-n);
		}
		break;
	case 'c':       /* DA: terminal type query */
		break;
	case 'n':       /* DSR: cursor position query */
		break;
	case 'h':       /* SM: toggle modes to high */
		if (m_term->check_compat(VT100))
		{
			get_control()->toggle_modes(true);
		}
		break;
	case 'i':		/* MC: Media copy */
		break;
	case 'l':       /* RM: toggle modes to low */
		if (m_term->check_compat(VT100))
		{
			get_control()->toggle_modes(false);
		}
		break;
	case 'g':       /* TBC: clear tabs */
		if (m_term->check_compat(VT100))
		{
			get_control()->clear_tabs();
		}
		break;
	case 'r':       /* DECSTBM: set scroll margins */
		if (m_term->check_compat(VT100))
		{
			get_control()->set_scroll_margins();
		}
		break;
	case 'm':       /* SGR: set graphics rendition */
		get_control()->set_graphics_rendition();
		break;
	case 's':       /* save cursor */
		get_control()->save_cursor(true);
		break;
	case 'u':       /* restore cursor */
		get_control()->save_cursor(false);
		break;
	case 't': /* DECSLPP: set page size - ie window height */
		get_control()->set_pagesize();
		break;
	case 'S':		/* SU: Scroll up */
		if (m_term->check_compat(SCOANSI))
		{
			screen* sc = get_screen();
			int lines = arg_list->get_arg(0, m_term->get_rows(), 1);
			m_term->scroll(sc->get_margin(), lines, true);
			sc->set_wrap_next(false);
		}
		break;
	case 'T':		/* SD: Scroll down */
		if (m_term->check_compat(SCOANSI))
		{
			screen* sc = get_screen();
			int lines = arg_list->get_arg(0, m_term->get_rows(), 1);
			m_term->scroll(sc->get_margin(), -lines, true);
			sc->set_wrap_next(false);
		}
		break;
	case 'X':     /* ECH: write N spaces w/o moving cursor */
		if (m_term->check_compat(ANSIMIN))
		{
			int cols = m_term->get_cols();
			int n = arg_list->get_arg(0, cols, 1);
			n = std::min<int>(n, cols - old_curs.col);
			
			screen* sc = get_screen();
			cursor curs_old = sc->get_cursor();
			termline_ptr cline = sc->get_lines()->get_line(curs_old.row);

			int p = curs_old.col;
			termchar* erase_char = m_term->get_erase_char();
			while (n--)
			{
				get_control()->copy_termchar(cline.get(), p++, erase_char);
			}
		}
		break;
	case 'x':       /* DECREQTPARM: report terminal characteristics */
		break;
	case 'Z':		/* CBT */
		if (m_term->check_compat(OTHER))
		{
			int arg = arg_list->get_arg(0, m_term->get_rows(), 1);
			screen* sc = get_screen();
			cursor old_curs = sc->get_cursor();

			for (int i = arg; i > 0; i--) 
			{
				cursor temp_curs = sc->get_cursor();
				if (temp_curs.col <= 0)
				{
					break;
				}

				int temp = 0;
				m_term->find_next_tab(old_curs.col, temp);

				temp_curs.col = temp;
				sc->set_cursor(temp_curs);
			}
		}
		break;
	}
}

void parser_csi::parser_comb(char query, char ch)
{
	argument* arg_list = m_term->get_argument();
	if (query == 1)
	{
		switch (ch)
		{
		case 'h':
			if (m_term->check_compat(VT100))
			{
				get_control()->toggle_modes(true);
			}
			break;
		case 'i':
			break;
		case 'l':
			if (m_term->check_compat(VT100))
			{
				get_control()->toggle_modes(false);
			}
			break;
		default:
			break;
		}
	}
	else if (query == '>')
	{
		if (ch == 'c') /* DA: report xterm version */
		{
		}
	}
	else if (query == '*')
	{
		if (ch == '|') /* DECSNLS */
		{
			if (m_term->check_compat(VT420))
			{

			}
		}
	}
	else if (query == '$')
	{
		if (ch == '|') /* DECSCPP */
		{
			
		}
	}
	else if (query == '=')
	{
		switch (ch)
		{
		case 'c':      /* Hide or Show Cursor */
		{
			int arg = arg_list->get_arg(0);
			switch (arg)
			{
			case 0:
				get_conf()->set_cursor_on(false);
				break;
			case 1:
				get_conf()->set_cursor_on(true);
				break;
			case 2:
				get_conf()->set_cursor_on(true);
				break;
			}
			break;
		}
		case 'C':
			if (m_term->check_compat(SCOANSI))
			{
				if (arg_list->count() >= 2)
				{
					bool cursor_on = arg_list->get_arg(0) <= arg_list->get_arg(1);
					get_conf()->set_cursor_on(cursor_on);
				}
			}
			break;
		case 'D':
			if (m_term->check_compat(SCOANSI))
			{
				int arg = arg_list->get_arg(0);
				
			}
			break;
		case 'E':
			//m_render->SetBlinkIsReal();
			break;
		case 'F':      /* set normal foreground */
			//m_render->NormalForeground();
			break;
		case 'G':      /* set normal background */
			//m_render->NomalBackground();
			break;
		case 'L':
			//m_erase->UseBgcolorErase();
			break;
		default:
			break;
		}
	}
	else if (query == '"')
	{
		if (ch == 'p') /* DECSCL: set compat level */
		{
			int compatibility_level = m_term->get_compat();
			int arg = arg_list->get_arg(0);
			switch (arg) 
			{
			case 61:
				compatibility_level &= ~TM_VTXXX;
				compatibility_level |= TM_VT102;
				break;
			case 62:
				compatibility_level &= ~TM_VTXXX;
				compatibility_level |= TM_VT220;
				break;
			default:
				if (60 < arg && arg < 70)
				{
					compatibility_level |= TM_VTXXX;
				}
				break;
			case 40:
				compatibility_level &= TM_VTXXX;
				break;
			case 41:
				compatibility_level = TM_PUTTY;
				break;
			case 42:
				compatibility_level = TM_SCOANSI;
				break;
			case ARG_DEFAULT:
				compatibility_level = TM_PUTTY;
				break;
			case 50:
				break;
			}
			m_term->set_compat(compatibility_level);
		}
	}

	
}
