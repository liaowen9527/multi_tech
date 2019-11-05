#include "screen.h"
#include "terminal.h"

bool poslt(const cursor& _Left, const cursor& _Right)
{
	if (_Left.row == _Right.row)
	{
		return _Left.col < _Right.col;
	}
	return _Left.row < _Right.row;
}


screen::screen()
{
	m_curs.row = m_curs.col = 0;
	m_savecurs = m_curs;
	m_marg.top = m_marg.bottom = 0;
	m_utf = m_save_utf = true;
	m_wrap = true;
	m_wrapnext = false;
	m_insert = false;
	m_dec_om = false;

	m_cset = m_save_cset = 0;
	m_save_csattr = 0;
	m_sco_acs = m_save_sco_acs = 0;
	m_curr_attr = m_save_attr = 0;
}

screen::~screen()
{

}

void screen::set_term(terminal* term)
{
	m_term = term;
}

void screen::swap_screen(screen* alt_screen, bool reset, bool keep_cur_pos)
{
	if (reset)
	{
		alt_screen->m_curs = m_curs;
		alt_screen->m_savecurs = m_savecurs;

		alt_screen->m_marg = m_marg;

		alt_screen->m_utf = m_utf;
		alt_screen->m_save_utf = m_save_utf;
		alt_screen->m_wrap = m_wrap;
		alt_screen->m_wrapnext = m_wrapnext;
		alt_screen->m_save_wnext = m_save_wnext;
		alt_screen->m_insert = m_insert;

		alt_screen->m_cset = m_cset;
		alt_screen->m_save_cset = m_save_cset;
		alt_screen->m_save_csattr = m_save_csattr;
		alt_screen->m_save_attr = m_save_attr;

		alt_screen->m_sco_acs = m_sco_acs;
		alt_screen->m_save_sco_acs = m_save_sco_acs;
	}
	else
	{
		std::swap(alt_screen->m_marg, m_marg);

		std::swap(alt_screen->m_utf, m_utf);
		std::swap(alt_screen->m_wrap, m_wrap);
		std::swap(alt_screen->m_wrapnext, m_wrapnext);
		std::swap(alt_screen->m_insert, m_insert);

		std::swap(alt_screen->m_cset, m_cset);
		std::swap(alt_screen->m_sco_acs, m_sco_acs);

		
		if (keep_cur_pos)
		{
			alt_screen->m_curs = m_curs;
			alt_screen->m_savecurs = m_savecurs;
			alt_screen->m_save_utf = m_save_utf;
			alt_screen->m_save_wnext = m_save_wnext;

			alt_screen->m_save_cset = m_save_cset;
			alt_screen->m_save_csattr = m_save_csattr;
			alt_screen->m_save_attr = m_save_attr;

			alt_screen->m_save_sco_acs = m_save_sco_acs;
		}
		else
		{
			std::swap(alt_screen->m_curs, m_curs);
			std::swap(alt_screen->m_savecurs, m_savecurs);
			std::swap(alt_screen->m_save_utf, m_save_utf);
			std::swap(alt_screen->m_save_wnext, m_save_wnext);

			std::swap(alt_screen->m_save_cset, m_save_cset);
			std::swap(alt_screen->m_save_csattr, m_save_csattr);
			std::swap(alt_screen->m_save_attr, m_save_attr);

			std::swap(alt_screen->m_save_sco_acs, m_save_sco_acs);
		}
	}
	
}

void screen::save_cursor()
{
	m_savecurs = m_curs;
	m_save_utf = m_utf;
	m_save_attr = m_curr_attr;
	m_save_cset = m_cset;
	m_save_wnext = m_wrapnext;
	m_save_csattr = m_term->get_charset(m_cset);
	m_save_sco_acs = m_sco_acs;
}

void screen::restore_cursor()
{
	m_curs = m_savecurs;
	/* Make sure the window hasn't shrunk since the save */
	m_curs.row = std::min<int>(m_curs.row, m_term->get_rows() - 1);
	m_curs.col = std::min<int>(m_curs.col, m_term->get_cols() - 1);

	m_utf = m_save_utf;
	m_curr_attr = m_save_attr;
	m_cset = m_save_cset;
	m_wrapnext = (m_curs.col < m_term->get_cols() - 1) ? false : m_save_wnext;

	m_term->set_charset(m_cset, m_save_csattr);
	
	m_sco_acs = m_save_sco_acs;
}

cursor screen::get_cursor()
{
	return m_curs;
}

void screen::set_cursor(const cursor& curs)
{
	m_curs = curs;
}

void screen::set_curs_row(int row)
{
	m_curs.row = row;
}

void screen::set_curs_col(int col)
{
	m_curs.col = col;
}

margin screen::get_margin()
{
	return m_marg;
}

void screen::set_margin(const margin& marg)
{
	m_marg = marg;
}

bool screen::at_bottom()
{
	return m_curs.row == m_marg.bottom;
}

bool screen::get_utf()
{
	return m_utf;
}

void screen::set_utf(bool val)
{
	m_utf = val;
}

int screen::get_sco_acs()
{
	return m_sco_acs;
}

void screen::set_sco_acs(int val)
{
	m_sco_acs = val;
}

screen_lines* screen::get_lines()
{
	return &m_lines;
}

termchar_ptr screen::get_curs_char()
{
	return get_char(m_curs.row, m_curs.col);
}

termchar_ptr screen::get_char(int row, int col)
{
	termline_ptr line = m_lines.get_line(row);
	return line->get_char(col);
}

bool screen::can_wrap()
{
	return m_wrap;
}

void screen::set_wrap(bool val)
{
	m_wrap = val;
}

bool screen::is_wrap_next()
{
	return m_wrapnext;
}

void screen::set_wrap_next(bool val)
{
	m_wrapnext = val;
}

bool screen::is_dec_om()
{
	return m_dec_om;
}

void screen::set_dec_om(bool val)
{
	m_dec_om = val;
}

