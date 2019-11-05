#pragma once

class conf
{
public:
	conf();
	~conf();

public:
	bool is_vt52mode();
	void set_vt52mode(bool val);

	bool is_cursor_on();
	void set_cursor_on(bool val);

	bool is_blink_real();
	void set_blink_real(bool val);

	bool is_use_bce();
	void set_use_bce(bool val);

public:
	bool is_app_cursor_keys();
	void set_app_cursor_keys(bool val);

	bool is_app_keypad_keys();
	void set_app_keypad_keys(bool val);

	bool is_repeat_off();
	void set_repeat_off(bool val);

	bool is_cr_lf_return();
	void set_cr_lf_return(bool val);

public:
	bool can_blink_text();

	bool is_crhaslf();
	bool is_lfhascr();
	bool is_no_alt_screen();
	bool is_erase_to_scrollback();
	bool is_no_remote_charset();

protected:
	bool m_cursor_on;
	bool m_use_bce;	//use back color to erase

	bool m_vt52_mode;

	bool m_crhaslf;	
	bool m_lfhascr;

	bool m_no_alt_screen;
	bool m_erase_to_scrollback;

	bool m_blink_text;
	bool m_blink_is_real;
	bool m_no_remote_charset;

	//for send command to device
	bool m_app_cursor_keys;
	bool m_app_keypad_keys;
	bool m_repeat_off;
	bool m_srm_echo;
	bool m_cr_lf_return;
};