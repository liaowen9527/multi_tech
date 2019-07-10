#pragma once
#include <vector>
#include "lw_ui_lib.h"
#include "TerminalPaintManager.h"

namespace lw_ui
{

class LWUI_API TerminalDelegate
{
public:
	virtual void GetLine(int nTextRow, std::vector<TextBlock>& vecBlock) = 0;
	virtual int GetTotalLines() = 0;
};

class LWUI_API CTerminal : public CStatic
{
public:
	CTerminal();
	~CTerminal();

public:
	TerminalPaintManager* GetPaintManager();
	void SetPaintManager(TerminalPaintManager* pPaintManager);

	TerminalDelegate* GetDelegate();
	void SetDelegate(TerminalDelegate* pDelegate);

	void ScrollToLine(int nLine);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

protected:
	void GetVisibleLines(int& nFirst, int& nLast);
	virtual int GetVisibleRowsCount(BOOL bExtend = TRUE);
	virtual int GetMaxLines();

	void RecalcScrollBars();

private:
	TerminalPaintManager* m_pPaintManager;
	TerminalDelegate* m_pDelegate;

	int m_offsetLine;

	BOOL m_bRecalcScroll;
	
};

}

