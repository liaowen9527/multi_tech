#pragma once
#include <vector>
#include <map>
#include <mutex>
#include <functional>
#include "lw_ui_lib.h"
#include "TerminalPaintManager.h"

namespace lw_ui
{

class LWUI_API TerminalDelegate
{
public:
	virtual void GetLine(int nTextRow, std::vector<TextBlock>& vecBlock) = 0;
	virtual int GetTotalLines() = 0;

public:
	virtual bool OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
	virtual bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) { return false; }
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

	void SetCurPos(int row, int col, BOOL bForceVisible = TRUE);
	void EnsureVisible(int row, int col);
	void ScrollToLine(int nLine);

	void ScheduleTimer(int ticks, std::function<void()> func);

protected:
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
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

	std::mutex m_mutex;
	CPoint m_curPos;
	int m_offsetLine;

	BOOL m_bRecalcScroll;
	
	std::map<time_t, std::function<void()>> m_jobs;
};

}

