#pragma once
#include "lw_ui_lib.h"

class LWUI_API CClipboard
{
public:
	CClipboard(CWnd* pWnd);
	~CClipboard();

public:
	void Copy(const CString& strContent);
	void Paste(CString& strText);

protected:
	CWnd* m_pWnd;
};


