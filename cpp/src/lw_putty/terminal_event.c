
#pragma once

#include "putty.h"

void request_resize(void *frontend, int w, int h)
{
}

void do_beep(void *frontend, int mode)
{
#ifdef WIN32
	Beep(800, 100);
#endif
}

void do_text(Context ctx, int x, int y, wchar_t *text, int len,
	unsigned long attr, int lattr)
{
}

void do_cursor(Context ctx, int x, int y, wchar_t *text, int len,
	unsigned long attr, int lattr)
{
}

int char_width(Context ctx, int uc) {
	return 0;
}

void set_title(void *frontend, char *title)
{
}

void set_icon(void *frontend, char *title)
{
}

void set_sbar(void *frontend, int total, int start, int page)
{
}

Context get_ctx(void *frontend)
{
	return NULL;
}

void free_ctx(Context ctx)
{
}

void write_clip(void *frontend, wchar_t * data, int *attr, int len, int must_deselect)
{
}

void set_raw_mouse_mode(void *frontend, int activate)
{
}

void sys_cursor(void *frontend, int x, int y)
{
}

void request_paste(void *frontend)
{
}

void get_clip(void *frontend, wchar_t **p, int *len)
{
}

int frontend_is_utf8(void *frontend)
{
	return 1;
}

void set_iconic(void *frontend, int iconic){}
void move_window(void *frontend, int x, int y){}
void set_zorder(void *frontend, int top){}
void refresh_window(void *frontend){}
void set_zoomed(void *frontend, int zoomed){}
int is_iconic(void *frontend) { return 1; }
void get_window_pos(void *frontend, int *x, int *y){}
void get_window_pixels(void *frontend, int *x, int *y){}
char *get_window_title(void *frontend, int icon) { return ""; }


void palette_set(void *frontend, int n, int r, int g, int b){}
void palette_reset(void *frontend){}


printer_job *printer_start_job(char *printer)
{
	return NULL;
}

void printer_job_data(printer_job *pj, void *data, int len)
{
}

void printer_finish_job(printer_job *pj)
{
}