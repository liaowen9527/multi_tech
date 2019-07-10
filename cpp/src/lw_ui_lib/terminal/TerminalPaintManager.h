#pragma once
#include "lw_ui_lib.h"

namespace lw_ui
{

class CTerminal;

struct LWUI_API TextBlock
{
	int nRow;
	CString strContext;
	COLORREF clrBack;
	COLORREF clrForeground;
};

class LWUI_API TerminalPaintManager
{
public:
	TerminalPaintManager(CTerminal* pTerminal = NULL);
	~TerminalPaintManager();

public:
	void SetFont(const CString& strFontName, int nFontSize);
	void SetFont(const LOGFONT& logfont);

public:
	void DrawBackground(CDC* pDC, const CRect& rc);
	void DrawLines(CDC* pDC, int nFirstLine, int nLastLine);
	void DrawCursor(CDC* pDC);

	void RecalcRowHeight(CDC* pDC);
	int GetRowHeight();

protected:
	void InitStyle();

protected:
	PROPERTY(COLORREF, m_clrBack, BkColor)
	PROPERTY(COLORREF, m_clrForeground, ForegroundColor)

protected:
	CTerminal* m_pTerminal;
	CFont m_font;
	int m_nRowHeight;
};

}
