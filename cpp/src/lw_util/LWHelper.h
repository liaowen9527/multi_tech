#pragma once


#define lw_va_to_string(fmt, str) \
	{	\
		va_list argList;	\
		va_start(argList, fmt);	\
		int len = _vscprintf(format, argList) + 1;	\
		if (len > 0)	\
		{	\
			str.resize(len);	\
			vsprintf_s((char*)(str.c_str()), len, fmt, argList);	\
		}	\
		vsprintf_s((char*)(strMsg.c_str()), len, fmt, argList);	\
		va_end(argList);	\
	}
	
