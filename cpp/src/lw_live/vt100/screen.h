#pragma once
#include "chars/screen_lines.h"

struct cursor
{
	int row;
	int col;
};
extern bool poslt(const cursor& _Left, const cursor& _Right);

struct margin
{
	int top;
	int bottom;
};

class terminal;

class screen
{
public:
	screen();
	~screen();

public:
	void set_term(terminal* term);
	void swap_screen(screen* alt_screen, bool reset, bool keep_cur_pos);
	void save_cursor();
	void restore_cursor();

public:
	cursor get_cursor();
	void set_cursor(const cursor& curs);
	
	void set_curs_row(int row);
	void set_curs_col(int col);

	margin get_margin();
	void set_margin(const margin& marg);

	bool at_bottom();

	bool get_utf();
	void set_utf(bool val);

	int get_sco_acs();
	void set_sco_acs(int val);

public:
	screen_lines* get_lines();
	termchar_ptr get_curs_char();
	termchar_ptr get_char(int row, int col);

	bool can_wrap();
	void set_wrap(bool val);

	bool is_wrap_next();
	void set_wrap_next(bool val);

	bool is_dec_om();
	void set_dec_om(bool val);

protected:
	terminal* m_term;
	screen_lines m_lines;
	cursor m_curs;
	cursor m_savecurs;

	margin m_marg;

	bool m_utf;
	bool m_save_utf;
	bool m_wrap;
	bool m_wrapnext;
	bool m_save_wnext;
	bool m_insert;
	bool m_dec_om;

	int m_cset;
	int m_save_cset;
	int m_save_csattr;
	int m_curr_attr;
	int m_save_attr;

	int m_sco_acs;
	int m_save_sco_acs;
};