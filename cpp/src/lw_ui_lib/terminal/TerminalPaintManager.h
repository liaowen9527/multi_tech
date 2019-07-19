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
	CDrawTextProcessor* GetTextProcessor();

	CFont* GetFont();
	void SetFont(const CString& strFontName, int nFontSize);
	void SetFont(const LOGFONT& logfont);

	BOOL IsShowLineNumber();
	void ShowLineNumber(BOOL bShow);

	void BeginPaint(CDC* pDC, const CRect& rcClient);

public:
	void DrawBackground(CDC* pDC);
	void DrawSelection(CDC* pDC, const CRect& rc);
	void DrawCursor(CDC* pDC, const CRect& rc, BOOL bActive);
	void DrawLines(CDC* pDC, int nFirstLine, int nLastLine);
	void DrawLineNum(CDC* pDC, int nY, int number);

protected:
	void InitStyle();

protected:
	PROPERTY(COLORREF, m_clrBack, BkColor)
	PROPERTY(COLORREF, m_clrSelection, SelectionColor)
	PROPERTY(COLORREF, m_clrForeground, ForegroundColor)
	PROPERTY(COLORREF, m_clrCursor, CursorColor)
	PROPERTY(COLORREF, m_clrBackNumer, NumberBkColor)
	PROPERTY(COLORREF, m_clrNumer, NumberColor)

protected:
	CTerminal* m_pTerminal;
	CFont m_font;
	CDrawTextProcessor m_textProcessor;

	CRect m_rcClient;
	BOOL m_bShowNumber;
	int m_nLineNumLength;
};

}
