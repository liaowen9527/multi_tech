#include "vtcallback.h"
#include "vtterminal.h"

#ifdef __cplusplus
extern "C" {
#include "vt_ex.h"
#endif // __cplusplus

	void do_clip_line(Terminal *term, void* termline)
	{
		CVtTerminal* pTerminal = CVtTerminal::Find(term);
		if (pTerminal)
		{
			CVtCallback* pCallback = pTerminal->GetCallback();
			if (pCallback)
			{
				pCallback->OnClipLine(termline);
			}
		}

	}

#ifdef __cplusplus
}
#endif // __cplusplus
