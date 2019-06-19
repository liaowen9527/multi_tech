#ifndef __VIRTUAL_TERMINAL_EXTEND_H__
#define __VIRTUAL_TERMINAL_EXTEND_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef struct terminal_tag Terminal;
	struct termline *lineptr2(Terminal *term, int line, int lineno);

	void do_clip_line(Terminal *term, void* termline);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __VIRTUAL_TERMINAL_EXTEND_H__
