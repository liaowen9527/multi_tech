#pragma once

class terminal;
class parser;
class argument;
class screen;
class conf;
class termline;
class termchar;
struct cursor;


class control
{
public:
	control();
	~control();

	void set_term(terminal* term);

public:
	void cr();
	void lf();
	void tab();
	void backspace();

	void display_graphic_char(termchar* pchr);
	int copy_termchar(termline* line, int col, termchar* pchr);
	void erase_lots(bool line_only, bool from_begin, bool to_end);

	void clear_tabs();
	void set_scroll_margins();

public:
	void move_cursor(int row, int col, int marg_clip);
	bool incpos(cursor* curs);

	void save_cursor(bool save = true);

public:
	void toggle_modes(bool state);
	void toggle_mode(int mode, int query, bool state);
	void toggle_mode_0(int mode, bool state);
	void toggle_mode_1(int mode, bool state);

	void set_graphics_rendition();
	void set_pagesize();

	void set_sco_acs(int val);
	void set_colours_256(bool backcolor, int& index_arg);

public:
	void into_esc();
	void into_csi();
	void into_osc();
	void into_osc_p();

protected:
	parser* get_parser();
	argument* get_argument();
	conf* get_conf();
	screen* get_screen();

protected:
	terminal* m_term;
};