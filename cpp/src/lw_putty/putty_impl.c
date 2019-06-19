#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#define PUTTY_DO_GLOBALS	       /* actually _define_ globals */
#include "putty.h"
#include "storage.h"
#include "tree234.h"
#include "winsecur.h"

#define WM_AGENT_CALLBACK (WM_APP + 4)


struct agent_callback {
	void(*callback)(void *, void *, int);
	void *callback_ctx;
	void *data;
	int len;
};

void fatalbox(const char *p, ...)
{

}
void modalfatalbox(const char *p, ...)
{

}
void nonfatal(const char *p, ...)
{

}

void cmdline_error(const char *p, ...)
{

}

void frontend_echoedit_update(void *frontend, int echo, int edit)
{
}

char *get_ttymode(void *frontend, const char *mode) { return NULL; }

int from_backend_untrusted(void *frontend_handle, const char *data, int len)
{
	/*
	 * No "untrusted" output should get here (the way the code is
	 * currently, it's all diverted by FLAG_STDERR).
	 */
	//assert(!"Unexpected call to from_backend_untrusted()");
	return 0; /* not reached */
}

int from_backend_eof(void *frontend_handle)
{
	//handle_write_eof(stdout_handle);
	return FALSE;   /* do not respond to incoming EOF with outgoing */
}

//int get_userpass_input(prompts_t *p, const unsigned char *in, int inlen)
//{
//	int ret;
//	ret = cmdline_get_passwd_input(p, in, inlen);
//	if (ret == -1)
//		ret = console_get_userpass_input(p, in, inlen);
//	return ret;
//}

void agent_schedule_callback(void(*callback)(void *, void *, int),
	void *callback_ctx, void *data, int len)
{
	struct agent_callback *c = snew(struct agent_callback);
	c->callback = callback;
	c->callback_ctx = callback_ctx;
	c->data = data;
	c->len = len;
	//PostThreadMessage(main_thread_id, WM_AGENT_CALLBACK, 0, (LPARAM)c);
}

char *do_select(SOCKET skt, int startup)
{
	int events;
	if (startup) {
		events = (FD_CONNECT | FD_READ | FD_WRITE |
			FD_OOB | FD_CLOSE | FD_ACCEPT);
	}
	else {
		events = 0;
	}

	WSAEVENT netevent = get_netevent(skt);
	if (p_WSAEventSelect(skt, netevent, events) == SOCKET_ERROR) {
		switch (p_WSAGetLastError()) {
		case WSAENETDOWN:
			return "Network is down";
		default:
			return "WSAEventSelect(): unknown error";
		}
	}
	return NULL;
}

const int share_can_be_downstream = TRUE;
const int share_can_be_upstream = TRUE;

