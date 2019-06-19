#ifndef __PUTTY_CONF_H__
#define __PUTTY_CONF_H__

#include <winsock2.h> 

typedef struct conf_tag Conf;

#ifdef __cplusplus
extern "C" {
#endif

	extern void SetTelnetMode(Conf* conf);
	extern void SetSshMode(Conf* conf);
	
	//for ssh
	extern void SetSshVer1(Conf* conf);
	extern void SetSshVer2(Conf* conf);

	extern void SetKeyFile(Conf* conf, const char* filepath);


#ifdef __cplusplus
}
#endif

#endif
