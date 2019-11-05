#include "control.h"
#include <assert.h>
#include "../conf/conf.h"
#include "../terminal.h"

control::control()
{

}

control::~control()
{

}

void control::set_term(terminal* term)
{
	m_term = term;
}

void control::cr()
{
	screen* sc = m_term->get_screen();
	sc->set_curs_col(0);
	sc->set_wrap_next(false);

	if (get_conf()->is_crhaslf())
	{
		m_term->nextline();
	}
}

void control::lf()
{
	m_term->nextline();

	screen* sc = m_term->get_screen();
	if (get_conf()->is_lfhascr())
	{
		sc->set_curs_col(0);
	}

	sc->set_wrap_next(false);
}

void control::tab()
{
	screen* sc = m_term->get_screen();
	screen_lines* sclines = sc->get_lines();
	cursor old_curs = sc->get_cursor();
	termline_ptr ldata = sclines->get_line(old_curs.row);

	int col_tab = 0;
	m_term->find_next_tab(old_curs.col, col_tab);

	int new_col = col_tab;
	int cols = m_term->get_cols();
	cols = ldata->is_width() ? cols / 2 : cols;
	if (col_tab >= cols)
	{
		new_col = cols - 1;
	}

	sc->set_curs_col(new_col);
}

void control::backspace()
{
	screen* sc = m_term->get_screen();
	cursor curs = sc->get_cursor();
	assert(curs.col >= 0 || curs.row >= 0);

	if (curs.col == 0)
	{
		if (curs.row > 0 && sc->can_wrap())
		{
			curs.row -= 1;
			curs.col = m_term->get_cols() - 1;
			sc->set_cursor(curs);
		}
	}
	else
	{
		if (sc->is_wrap_next())
		{
			sc->set_wrap_next(false);
		}
		else
		{
			sc->set_curs_col(curs.col - 1);
		}
	}
}

void control::display_graphic_char(termchar* pchr)
{
	screen* sc = m_term->get_screen();
	cursor curs = sc->get_cursor();

	termline_ptr line = sc->get_lines()->get_line(curs.row);
	curs.col = copy_termchar(line.get(), curs.col, pchr);

	sc->set_cursor(curs);
}

int control::copy_termchar(termline* line, int col, termchar* pchr)
{
	int col_temp = col;
	termchar_ptr chr = line->get_char(col_temp);
	chr->set_char(pchr->get_char());
	col_temp++;

	if (pchr->is_wchar())
	{
		chr = line->get_char(col_temp);
		chr->set_ucswide(true);
		col_temp++;
	}

	return col_temp;
}

void control::erase_lots(bool line_only, bool from_begin, bool to_end)
{
	screen* sc = m_term->get_screen();
	cursor old_curs = sc->get_cursor();
	cursor start, end;
	bool erase_lattr = false;
	bool erasing_lines_from_top = false;

	if (line_only) 
	{
		start.row = old_curs.row;
		start.col = 0;
		end = start;
		end.row += 1;
		erase_lattr = false;
	}
	else 
	{
		start.row = start.col = 0;
		end = start;
		end.row = m_term->get_rows();
		erase_lattr = true;
	}

	cursor boundary = old_curs;
	if (!from_begin) 
	{
		if (sc->is_wrap_next())
		{
			incpos(&boundary);
		}

		start = boundary;
	}
	if (!to_end) 
	{
		incpos(&boundary);
		end = boundary;
	}

	/* Lines scrolled away shouldn't be brought back on if the terminal
	 * resizes. */
	if (start.row == 0 && start.col == 0 && end.col == 0 && erase_lattr)
	{
		erasing_lines_from_top = true;
	}

	if (get_conf()->is_erase_to_scrollback() && erasing_lines_from_top) 
	{
		int scrolllines = end.row;
		if (end.row == m_term->get_rows())
		{
			scrolllines = sc->get_lines()->find_last_nonempty_line() + 1;
		}
		if (scrolllines > 0)
		{
			margin marg;
			marg.top = 0;
			marg.bottom = scrolllines - 1;
			m_term->scroll(marg, scrolllines, true);
		}	
	}
	else 
	{
		termline_ptr ldata = sc->get_lines()->get_line(start.row);
		termchar* erase_char = m_term->get_erase_char();
		while (poslt(start, end))
		{
			if (start.col == m_term->get_cols())
			{
				
			}
			else 
			{
				copy_termchar(ldata.get(), start.col, erase_char);
			}
			if (incpos(&start) && start.row < m_term->get_rows()) 
			{
				ldata = sc->get_lines()->get_line(start.row);
			}
		}
	}
}

void control::clear_tabs()
{
	argument* arg_list = m_term->get_argument();
	char query = arg_list->get_esc_query();
	if (query == 1) 
	{
		int arg = arg_list->get_arg(0);
		if (arg == 0)
		{
			cursor curs = m_term->get_screen()->get_cursor();
			m_term->set_a_tab(curs.col, false);
		}
		else if (arg == 3)
		{
			for (int i = 0; i < m_term->get_cols(); i++)
			{
				m_term->set_a_tab(i, false);
			}
		}
	}
}

void control::set_scroll_margins()
{
	argument* arg_list = m_term->get_argument();
	char query = arg_list->get_esc_query();
	if (query > 2)
	{
		return;
	}

	int rows = m_term->get_rows();
	int arg_1 = arg_list->get_arg(1, rows, rows);

	margin marg;
	marg.top = arg_list->get_arg(0, m_term->get_rows(), 1) - 1;
	marg.bottom = std::min<int>(arg_1 - 1, rows - 1);
	
	if (marg.bottom - marg.top > 0)
	{
		screen* sc = m_term->get_screen();
		sc->set_margin(marg);
		sc->set_curs_col(0);

		int row = sc->is_dec_om() ? marg.top : 0;
		sc->set_curs_row(row);
	}
}

void control::move_cursor(int row, int col, int marg_clip)
{
	col = std::max<int>(col, 0);
	col = std::min<int>(col, m_term->get_cols() - 1);

	screen* sc = m_term->get_screen();
	if (marg_clip) 
	{
		margin marg = sc->get_margin();
		bool force_clip = (marg_clip == 2);
		if (force_clip || row >= marg.top)
		{
			row = std::max<int>(row, marg.top);
		}
		if (force_clip || row <= marg.bottom)
		{
			row = std::min<int>(row, marg.bottom);
		}
	}
	
	row = std::max<int>(row, 0);
	row = std::min<int>(row, m_term->get_rows() - 1);

	sc->set_curs_row(row);
	sc->set_curs_col(col);
	sc->set_wrap_next(false);
}

bool control::incpos(cursor* curs)
{
	int cols = m_term->get_cols();
	if (curs->col == cols)
	{
		curs->col = 0;
		curs->row++;
		return true;
	}

	curs->col++;
	return false;
}

void control::save_cursor(bool save/* = true*/)
{
	if (save)
	{
		if (m_term->check_compat(VT100))
		{
			m_term->get_screen()->save_cursor();
		}
	}
	else
	{
		m_term->get_screen()->restore_cursor();
	}
}

void control::toggle_modes(bool state)
{
	argument* arg_list = m_term->get_argument();
	char query = arg_list->get_esc_query();

	for (int i = 0; i < arg_list->count(); i++)
	{
		int arg = arg_list->get_arg(i);
		toggle_mode(arg, query, state);
	}
}

void control::toggle_mode(int mode, int query, bool state)
{
	if (query == 0)
	{
		toggle_mode_0(mode, state);
	}
	else if (query == 1) 
	{
		toggle_mode_1(mode, state);
	}
}

void control::toggle_mode_0(int mode, bool state)
{
	switch (mode)
	{
	case 4:                      /* IRM: set insert mode */
		if (m_term->check_compat(VT102))
		{
			//term->insert = state;
		}
		break;
	case 12:                     /* SRM: set echo mode */
		//term->srm_echo = !state;
		break;
	case 20:                     /* LNM: Return sends ... */
		get_conf()->set_cr_lf_return(state);
		break;
	case 34:                     /* WYULCURM: Make cursor BIG */
		if (m_term->check_compat_2(OTHER, VT220))
		{
			//term->big_cursor = !state;
		}
	}
}

void control::toggle_mode_1(int mode, bool state)
{
	switch (mode) 
	{
	case 1:                      /* DECCKM: application cursor keys */
		get_conf()->set_app_cursor_keys(state);
		break;
	case 2:                      /* DECANM: VT52 mode */
	{
		conf* pconf = get_conf();
		pconf->set_vt52mode(!state);
		if (pconf->is_vt52mode())
		{
			pconf->set_blink_real(false);
		}
		else
		{
			pconf->set_blink_real(pconf->can_blink_text());
		}
		break;
	}
	case 3:                      /* DECCOLM: 80/132 columns */
	{
		break;
	}
	case 5:                      /* DECSCNM: reverse video */
		break;
	case 6:                      /* DECOM: DEC origin mode */
		get_screen()->set_dec_om(state);
		break;
	case 7:                      /* DECAWM: auto wrap */
		get_screen()->set_wrap(state);
		break;
	case 8:                      /* DECARM: auto key repeat */
		get_conf()->set_repeat_off(!state);
		break;
	case 25:                     /* DECTCEM: enable/disable cursor */
		if (m_term->check_compat_2(OTHER, VT220))
		{
			get_conf()->set_cursor_on(state);
		}
		break;
	case 47:                     /* alternate screen */
		if (m_term->check_compat(OTHER))
		{
			int witch = get_conf()->is_no_alt_screen() ? 0 : state;
			m_term->swap_screen(witch, false, false);
		}
		break;
	case 1000:                   /* xterm mouse 1 (normal) */
		break;
	case 1002:                   /* xterm mouse 2 (inc. button drags) */
		break;
	case 1006:                   /* xterm extended mouse */
		break;
	case 1015:                   /* urxvt extended mouse */
		break;
	case 1047:                   /* alternate screen */
		if (m_term->check_compat(OTHER))
		{
			int witch = get_conf()->is_no_alt_screen() ? 0 : state;
			m_term->swap_screen(witch, true, true);
		}
		break;
	case 1048:                   /* save/restore cursor */
		if (!get_conf()->is_no_alt_screen())
		{
			save_cursor(state);
		}
		break;
	case 1049:                   /* cursor & alternate screen */
	{
		if (state && !get_conf()->is_no_alt_screen())
		{
			save_cursor(state);
		}

		if (!m_term->check_compat(OTHER))
		{
			break;
		}
		
		int witch = get_conf()->is_no_alt_screen() ? 0 : state;
		m_term->swap_screen(witch, true, false);

		if (!state && !get_conf()->is_no_alt_screen())
		{
			save_cursor(state);
		}

		break;
	}
	case 2004:                   /* xterm bracketed paste */
		//term->bracketed_paste = state ? true : false;
		break;
	}
}

void control::set_graphics_rendition()
{
	argument* arg_list = m_term->get_argument();
	termchar_attr* default_attr = m_term->get_default_attr();
	termchar_attr* curr_attr = m_term->get_curr_attr();

	for (int i = 0; i < arg_list->count(); i++)
	{
		unsigned int arg = arg_list->get_arg(i, 0);
		switch (arg)
		{
		case 0:       /* restore defaults */
			m_term->restore_attr();
			break;
		case 1:       /* enable bold */
			if (m_term->check_compat(VT100AVO))
			{
				curr_attr->enable(ATTR_BOLD);
			}
			break;
		case 2:       /* enable dim */
			if (m_term->check_compat(OTHER))
			{
				curr_attr->enable(ATTR_DIM);
			}
			break;
		case 21:      /* (enable double underline) */
			if (!m_term->check_compat(OTHER))
			{
				break;
			}
		case 4:       /* enable underline */
			if (m_term->check_compat(VT100AVO))
			{
				curr_attr->enable(ATTR_UNDER);
			}
			break;
		case 5:       /* enable blink */
			if (m_term->check_compat(VT100AVO))
			{
				curr_attr->enable(ATTR_BLINK);
			}
			break;
		case 6:       /* SCO light bkgrd */
			if (m_term->check_compat(SCOANSI))
			{
				get_conf()->set_blink_real(false);
				curr_attr->enable(ATTR_BLINK);
			}
			break;
		case 7:       /* enable reverse video */
			curr_attr->enable(ATTR_REVERSE);
			break;
		case 10:      /* SCO acs off */
			set_sco_acs(0);
			break;
		case 11:      /* SCO acs on */
			set_sco_acs(1);
			break;
		case 12:      /* SCO acs on, |0x80 */
			set_sco_acs(2);
			break;
		case 22:      /* disable bold and dim */
			if (m_term->check_compat(OTHER) && m_term->check_compat(VT220))
			{
				curr_attr->enable(ATTR_BOLD | ATTR_DIM, false);
			}
			break;
		case 24:      /* disable underline */
			if (m_term->check_compat(OTHER) && m_term->check_compat(VT220))
			{
				curr_attr->enable(ATTR_UNDER, false);
			}
			break;
		case 25:      /* disable blink */
			if (m_term->check_compat(OTHER) && m_term->check_compat(VT220))
			{
				curr_attr->enable(ATTR_BLINK, false);
			}
			break;
		case 27:      /* disable reverse video */
			if (m_term->check_compat(OTHER) && m_term->check_compat(VT220))
			{
				curr_attr->enable(ATTR_REVERSE, false);
			}
			break;
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
			/* foreground */
			curr_attr->set_colour(arg - 30, false);
			break;
		case 90:
		case 91:
		case 92:
		case 93:
		case 94:
		case 95:
		case 96:
		case 97:
			/* aixterm-style bright foreground */
			curr_attr->set_colour(arg - 90 + 8, false);
			break;
		case 39:      /* default-foreground */
			curr_attr->set_colour_default(false);
			break;
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
			/* background */
			curr_attr->set_colour(arg - 40, true);
			break;
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
			/* aixterm-style bright background */
			curr_attr->set_colour(arg - 100 + 8, true);
			break;
		case 49:      /* default-background */
			curr_attr->set_colour_default(true);
			break;
		case 38:
			set_colours_256(false, i);
			break;
		case 48:
			set_colours_256(true, i);
			break;
		}
	}
	m_term->set_erase_char();
}

void control::set_pagesize()
{

}

void control::set_sco_acs(int val)
{
	if (m_term->check_compat(SCOANSI))
	{
		if (get_conf()->is_no_remote_charset())
		{
			return;
		}

		get_screen()->set_sco_acs(0);
	}
}

void control::set_colours_256(bool backcolor, int& index_arg)
{
	argument* arg_list = m_term->get_argument();
	termchar_attr* default_attr = m_term->get_default_attr();
	termchar_attr* curr_attr = m_term->get_curr_attr();

	int i = index_arg;
	int nargs = arg_list->count();
	if (nargs <= i + 2)
	{
		return;
	}

	do 
	{
		//sequence of 3 arguments in the form 38; 5; n, where n is in the range 0 - 255. A true - colour RGB
		int arg_1 = arg_list->get_arg(i + 1);
		if (arg_1 == 5)
		{
			int arg_2 = arg_list->get_arg(i + 2);
			curr_attr->set_colour(arg_2 & 0xFF, backcolor);

			i += 2;
		}
	} while (0);
	
	if (nargs <= i + 4)
	{
		return;
	}

	//triple is selected by 5 args of the form 38; 2; r; g; b.Replacing the initial 38 with 48 in both
	do 
	{
		int arg_1 = arg_list->get_arg(i + 1);
		if (arg_1 == 2)
		{
			int r = arg_list->get_arg(i + 2);
			int g = arg_list->get_arg(i + 3);
			int b = arg_list->get_arg(i + 4);
			curr_attr->set_truecolour(r, g, b, backcolor);
			
			i += 4;
		}

	} while (0);
	
	index_arg = i;
}

void control::into_esc()
{
	if (m_term->get_conf()->is_vt52mode())
	{
		m_term->get_parser()->set_termstate(VT52_ESC);
	}
	else 
	{
		if (m_term->check_compat(ANSIMIN))
		{
			m_term->get_parser()->set_termstate(SEEN_ESC);
			m_term->get_argument()->set_esc_query(0);
		}
	}
}

void control::into_csi()
{
	m_term->get_parser()->set_termstate(SEEN_CSI);
	m_term->get_argument()->resize(1);
	m_term->get_argument()->set_arg(0, ARG_DEFAULT);
	m_term->get_argument()->set_esc_query(0);
}

void control::into_osc()
{
	/* Compatibility is nasty here, xterm, linux, decterm yuk! */
	if (m_term->check_compat(OTHER))
	{
		m_term->get_parser()->set_termstate(SEEN_OSC);
		m_term->get_argument()->resize(1);
		m_term->get_argument()->set_arg(0, 0);
	}	
}

void control::into_osc_p()
{
	m_term->get_parser()->set_termstate(SEEN_OSC_P);
}

parser* control::get_parser()
{
	return m_term->get_parser();
}

argument* control::get_argument()
{
	return m_term->get_argument();
}

conf* control::get_conf()
{
	return m_term->get_conf();
}

screen* control::get_screen()
{
	return m_term->get_screen();
}
