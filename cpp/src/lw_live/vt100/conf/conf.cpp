#include "conf.h"

conf::conf()
{
	m_cursor_on = true;
	m_use_bce = true;
	m_vt52_mode = false;

	m_blink_text = false;
	m_blink_is_real = false;
	m_no_remote_charset = false;

	m_crhaslf = false;
	m_lfhascr = false;
	m_no_alt_screen = false;
	m_erase_to_scrollback = true;

	m_app_cursor_keys = false;
	m_app_keypad_keys = false;
	m_repeat_off = false;
	m_srm_echo = false;
	m_cr_lf_return = false;
}

conf::~conf()
{

}

bool conf::is_vt52mode()
{
	return m_vt52_mode;
}

void conf::set_vt52mode(bool val)
{
	m_vt52_mode = val;
}

bool conf::is_cursor_on()
{
	return m_cursor_on;
}

void conf::set_cursor_on(bool val)
{
	m_cursor_on = val;
}

bool conf::is_blink_real()
{
	return m_blink_is_real;
}

void conf::set_blink_real(bool val)
{
	m_blink_is_real = val;
}

bool conf::is_use_bce()
{
	return m_use_bce;
}

void conf::set_use_bce(bool val)
{
	m_use_bce = val;
}

bool conf::is_app_cursor_keys()
{
	return m_app_cursor_keys;
}

void conf::set_app_cursor_keys(bool val)
{
	m_app_cursor_keys = val;
}

bool conf::is_app_keypad_keys()
{
	return m_app_keypad_keys;
}

void conf::set_app_keypad_keys(bool val)
{
	m_app_keypad_keys = val;
}

bool conf::is_repeat_off()
{
	return m_repeat_off;
}

void conf::set_repeat_off(bool val)
{
	m_repeat_off = val;
}

bool conf::is_cr_lf_return()
{
	return m_cr_lf_return;
}

void conf::set_cr_lf_return(bool val)
{
	m_cr_lf_return = val;
}

bool conf::can_blink_text()
{
	return m_blink_text;
}

bool conf::is_crhaslf()
{
	return m_crhaslf;
}

bool conf::is_lfhascr()
{
	return m_lfhascr;
}

bool conf::is_no_alt_screen()
{
	return m_no_alt_screen;
}

bool conf::is_erase_to_scrollback()
{
	return m_erase_to_scrollback;
}

bool conf::is_no_remote_charset()
{
	return m_no_remote_charset;
}
