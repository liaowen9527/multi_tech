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
	virtual CString GetLineText(int nLine) = 0;
	virtual void GetLine(int nLine, std::vector<TextBlock>& vecBlock) = 0;
	virtual int GetTotalLines() = 0;

	virtual CString GetWindowText(const CPoint& ptStart, const CPoint& ptEnd) = 0;
	virtual void GetCursorPos(int& row, int& col) = 0;
	virtual int GetVScrollBottom() = 0;
	virtual void SetVScrollBottom(int bottom) = 0;

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
	void SyncFromDelegate();

	void SetCurPos(int row, int col, BOOL bForceVisible = TRUE);
	void SelectWords(CPoint ptStart, CPoint ptEnd);

	void EnsureVisible(int row, int col);
	void ScrollToLine(int nLine);

	void ScheduleTimer(int ticks, std::function<void()> func);

	void Copy();
	CString GetSelectionText();

	BOOL IsSelected(int row);
	BOOL IsSelected(int row, int col);

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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
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

	void SelectWord(CPoint point);
	BOOL RowColFromPoint(CPoint pt, int *pRow, int *pCol, int *pDispRow = NULL, int *pDispCol = NULL);

	void RecalcScrollBars();

	BOOL GetSelectionRegion(CPoint& ptStart, CPoint& ptEnd);
	std::vector<CRect> GetSelectionRegion(CDC* pDC);
	CRect GetCursorRect(CDC* pDC);

	BOOL HitTest(const CPoint& pt, int& rnRow, int& rnCol);

	CRect GetRowVisibleRect(int nRow, int nRowCount = 1);
	CRect GetVisibleRect(CDC* pDC, int nRow, int nCol);
	CRect GetVisibleRect(CDC* pDC, int nRow, int nCol1, int nCol2);

	int GetVscrollPos();
	void SetVscrollPos(int vpos, bool backend = true);

private:
	std::mutex m_mutex;

	TerminalPaintManager* m_pPaintManager;
	TerminalDelegate* m_pDelegate;

	CPoint m_curPos;
	CPoint m_selectionStart;
	CPoint m_selectionEnd;
	BOOL m_bSelection;

	int m_vscrollPos;
	int m_visibleLines;
	
	std::map<time_t, std::function<void()>> m_jobs;
};

}

