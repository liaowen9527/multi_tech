#pragma once
#include "lw_ui_lib.h"
#include "DrawTextProcessor.h"

namespace lw_ui
{

class CTerminal;

struct LWUI_API TextBlock
{
	int nRow;
	CString strContext;
	COLORREF clrForeground;
};

class LWUI_API TerminalPaintManager
{
public:
	TerminalPaintManager(CTerminal* pTerminal = NULL);
	~TerminalPaintManager();

public:
	CFont* GetFont();
	void SetFont(const CString& strFontName, int nFontSize);
	void SetFont(const LOGFONT& logfont);

	CDrawTextProcessor* GetTextProcessor();

public:
	void DrawBackground(CDC* pDC, const CRect& rc);
	void DrawSelection(CDC* pDC, const CRect& rc);
	void DrawCursor(CDC* pDC, const CRect& rc, BOOL bActive);
	void DrawLines(CDC* pDC, int nFirstLine, int nLastLine);

protected:
	void InitStyle();

protected:
	PROPERTY(COLORREF, m_clrBack, BkColor)
	PROPERTY(COLORREF, m_clrSelection, SelectionColor)
	PROPERTY(COLORREF, m_clrForeground, ForegroundColor)
	PROPERTY(COLORREF, m_clrCursor, CursorColor)

protected:
	CTerminal* m_pTerminal;
	CFont m_font;
	CDrawTextProcessor m_textProcessor;
};

}
