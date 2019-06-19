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


void SetTelnetMode(Conf* conf)
{
	conf_set_int(conf, CONF_protocol, PROT_TELNET);
}

void SetSshMode(Conf* conf)
{
	conf_set_int(conf, CONF_protocol, PROT_SSH);
}

void SetSshVer1(Conf* conf)
{
	conf_set_int(conf, CONF_sshprot, 0);/* SSH-1 only */
}

void SetSshVer2(Conf* conf)
{
	conf_set_int(conf, CONF_sshprot, 3);/* SSH-2 only */
}

extern void SetKeyFile(Conf* conf, const char* filepath)
{
	Filename *fn = filename_from_str(filepath);
	conf_set_filename(conf, CONF_keyfile, fn);
	filename_free(fn);
}
