#pragma once
#include "lw_ui_lib.h"

class LWUI_API CFontDC
{
public:
	CFontDC(CDC* pDC, CFont* pFont = NULL);
	CFontDC(CDC* pDC, CFont* pFont, COLORREF clrTextColor);
	~CFontDC();

public:
	void SetFont(CFont* pFont);
	void SetColor(COLORREF clrTextColor);

	void SetFontColor(CFont* pFont, COLORREF clrTextColor);

	void ReleaseFont();

	void ReleaseColor();

protected:
	CDC*     m_pDC;             // Stored pointer to a device context.
	CFont*   m_pOldFont;        // Stored pointer to an initial font from the device context.
	COLORREF m_clrOldTextColor; // Stored an initial text color from the device context.
};


