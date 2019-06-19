#ifndef __PUTTY_IMPL_H__
#define __PUTTY_IMPL_H__

#include <winsock2.h> 
#include <string>

typedef struct conf_tag Conf;

#ifdef __cplusplus
extern "C" {
#endif

	WSAEVENT get_netevent(int new_event);


#ifdef __cplusplus
}
#endif

#endif
