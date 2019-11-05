#pragma once
#include <vector>
#include <queue>
#include "screen.h"
#include "parser/parser.h"
#include "control/control.h"
#include "conf/conf.h"
#include "chars/scrollback.h"
#include "vt_type.h"

class terminal
{
public:
	terminal();
	~terminal();

public:
	bool check_compat(int vttype);
	bool check_compat_2(int vttype, int vttype2);

	bool is_compat(int vttype);
	int get_compat();
	void set_compat(int vttype);

	parser* get_parser();
	control* get_control();
	conf* get_conf();
	argument* get_argument();
	screen_lines* get_sclines();

	scrollback* get_sb();
	screen* get_screen();
	bool had_alt_screen();

	int get_rows();
	int get_cols();

	int get_charset(int index);
	void set_charset(int index, int charset);

	termchar_ptr get_last_graphic_char();
	void set_last_graphic_char(termchar* chr);

	termchar* get_basic_erase_char();
	termchar* get_erase_char();
	void set_erase_char();

	termchar_attr* get_default_attr();
	termchar_attr* get_curr_attr();

	bool find_next_tab(int from_col, int& end_col);
	void set_a_tab(int col, bool val);

	bool is_support_utf();
	void support_utf(bool val);

public:
	void resize(int rows, int cols);
	void restore_attr();
	void swap_screen(int witch, bool reset, bool keep_cur_pos);
	void preline();
	void nextline();
	void inset_lines(int nlines);
	void delete_lines(int nlines);
	void scroll(margin marg, int lines, bool sb);
	void scroll_up(margin marg, int lines);

	void insch(int n);

protected:
	int m_compatibility_level;
	parser m_parser;
	control m_ctrl;
	conf m_conf;

	scrollback m_sb;
	std::vector<screen> m_screens;
	int m_alt_which;

	int m_rows;
	int m_cols;

	std::vector<bool> m_tabs;

	termchar m_basic_erase_char;
	termchar m_erase_char;	//to erase char
	termchar_ptr m_last_graphic_char;
	termchar_attr m_default_attr;
	termchar_attr m_curr_attr;
};