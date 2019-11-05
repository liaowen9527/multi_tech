#include "Terminal.h"
#include <algorithm>
#include "Clipboard.h"
#include "FontDC.h"

#define TIMER_SCHEDULE 100
#define TIMER_SELECTION_ID 101


namespace lw_ui
{

	CTerminal::CTerminal()
	{
		m_pPaintManager = new TerminalPaintManager(this);
		m_pDelegate = NULL;
		m_vscrollPos = 0;
		m_bSelection = FALSE;
	}

	CTerminal::~CTerminal()
	{
		SetPaintManager(NULL);
	}

	LRESULT CTerminal::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCHITTEST || message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK)
			return ::DefWindowProc(m_hWnd, message, wParam, lParam);

		return __super::WindowProc(message, wParam, lParam);
	}

	BOOL CTerminal::PreCreateWindow(CREATESTRUCT& cs)
	{
		cs.dwExStyle = cs.dwExStyle | SS_NOTIFY | WS_TABSTOP;
		return __super::PreCreateWindow(cs);
	}

	void CTerminal::PreSubclassWindow()
	{
		__super::PreSubclassWindow();

		SetTimer(TIMER_SCHEDULE, 1, NULL);
	}

	BEGIN_MESSAGE_MAP(CTerminal, CStatic)
		ON_WM_PAINT()
		ON_WM_SIZE()
		ON_WM_TIMER()
		ON_WM_LBUTTONDOWN()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEWHEEL()
		ON_WM_GETDLGCODE()
		ON_WM_KEYDOWN()
		ON_WM_CHAR()
		ON_WM_VSCROLL()
		ON_WM_SETCURSOR()
	END_MESSAGE_MAP()

	TerminalPaintManager* CTerminal::GetPaintManager()
	{
		return m_pPaintManager;
	}

	void CTerminal::SetPaintManager(TerminalPaintManager* pPaintManager)
	{
		SAFE_DELETE(m_pPaintManager);
		m_pPaintManager = pPaintManager;
	}

	TerminalDelegate* CTerminal::GetDelegate()
	{
		return m_pDelegate;
	}

	void CTerminal::SetDelegate(TerminalDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	void CTerminal::SetCurPos(int row, int col, BOOL bForceVisible/* = TRUE*/)
	{
		m_curPos.y = row;
		m_curPos.x = col;
		
		if (bForceVisible)
		{
			EnsureVisible(row, col);
		}
	}

	void CTerminal::SelectWords(CPoint ptStart, CPoint ptEnd)
	{
		m_selectionStart = ptStart;
		m_selectionEnd = ptEnd;
	}

	void CTerminal::EnsureVisible(int row, int col)
	{
		auto func = [this, row]() {
			SCROLLINFO info;
			ZeroMemory(&info, sizeof(SCROLLINFO));
			info.cbSize = sizeof(SCROLLINFO);
			info.fMask = SIF_ALL;

			GetScrollInfo(SB_VERT, &info);

			if (row < info.nPos)
			{
				ScrollToLine(row);
			}
			else if (info.nPage > 0 && row - info.nPage >= info.nPos)
			{
				ScrollToLine(row - info.nPage + 1);
			}
		};

		ScheduleTimer(0, func);
	}

	void CTerminal::ScrollToLine(int nLine)
	{
		if (nLine == m_vscrollPos)
		{
			return;
		}

		SCROLLINFO info;
		ZeroMemory(&info, sizeof(SCROLLINFO));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;

		GetScrollInfo(SB_VERT, &info);

		nLine = std::min<int>(nLine, info.nMax + 1 - info.nPage);
		nLine = std::max<int>(nLine, info.nMin);

		m_vscrollPos = nLine;

		SetScrollPos(SB_VERT, m_vscrollPos);

		Invalidate();
	}

	void CTerminal::ScheduleTimer(int ticks, std::function<void()> func)
	{
		time_t now = GetTickCount();
		time_t when = now + ticks;

		std::lock_guard<std::mutex> lck(m_mutex);
		m_jobs[when] = func;
	}

	void CTerminal::Copy()
	{
		CString strContent = GetSelectionText();

		CClipboard board(this);
		board.Copy(strContent);
	}

	CString CTerminal::GetSelectionText()
	{
		CPoint ptStart, ptEnd;
		if (!GetSelectionRegion(ptStart, ptEnd))
		{
			return _T("");
		}

		return m_pDelegate->GetWindowText(ptStart, ptEnd);
	}

	BOOL CTerminal::IsSelected(int row)
	{
		if (row == m_curPos.y)
		{
			return TRUE;
		}

		CPoint ptStart, ptEnd;
		if (!GetSelectionRegion(ptStart, ptEnd))
		{
			return FALSE;
		}

		return row >= ptStart.y && row <= ptEnd.y;
	}

	BOOL CTerminal::IsSelected(int row, int col)
	{
		if (row == m_curPos.y && col == m_curPos.x)
		{
			return TRUE;
		}

		CPoint ptStart, ptEnd;
		if (!GetSelectionRegion(ptStart, ptEnd))
		{
			return FALSE;
		}

		if (row < ptStart.y || row > ptEnd.y)
		{
			return FALSE;
		}
		else
		{
			if (ptStart.y == ptEnd.y)
			{
				return row == ptStart.y && col >= ptStart.x && col < ptEnd.x;
			}
			else
			{
				if (row == ptStart.y)
				{
					return col >= ptStart.x;
				}
				else if (row == ptEnd.y)
				{
					return col < ptEnd.x;
				}
			}
		}

		return TRUE;
	}

	void CTerminal::OnPaint()
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CPaintDC dc(this); // device context for painting
		CMemDC memDC(dc, rcClient);

		CDC* pDC = &memDC.GetDC();
		pDC->SelectObject(m_pPaintManager->GetFont());

		m_pPaintManager->BeginPaint(pDC, rcClient);
		m_pPaintManager->DrawBackground(pDC);

		std::vector<CRect> vecRect = GetSelectionRegion(pDC);
		for (CRect& rc : vecRect)
		{
			m_pPaintManager->DrawSelection(pDC, rc);
		}

		HWND hwndFocus = ::GetFocus();
		m_pPaintManager->DrawCursor(pDC, GetCursorRect(pDC), hwndFocus == this->GetSafeHwnd());

		int nFirst = 0;
		int nLast = 0;
		GetVisibleLines(nFirst, nLast);

		m_pPaintManager->DrawLines(pDC, nFirst, nLast);

		RecalcScrollBars();

		m_pPaintManager->GetTextProcessor()->SetDC(GetDC());
	}

	void CTerminal::OnSize(UINT nType, int cx, int cy)
	{
		__super::OnSize(nType, cx, cy);
	}

	void CTerminal::OnTimer(UINT_PTR nIDEvent)
	{
		__super::OnTimer(nIDEvent);

		switch (nIDEvent)
		{
		case TIMER_SCHEDULE:
		{
			time_t now = GetTickCount();
			std::lock_guard<std::mutex> lck(m_mutex);
			auto itrEnd = m_jobs.upper_bound(now);
			for (auto itr = m_jobs.begin(); itr != itrEnd;)
			{
				(itr->second)();
				itr = m_jobs.erase(itr);
			}
			break;
		}
		case TIMER_SELECTION_ID:
			break;
		default:
			break;
		}
		
	}

	void CTerminal::OnLButtonDown(UINT nFlags, CPoint point)
	{
		__super::OnLButtonDown(nFlags, point);

		SetFocus();

		m_bSelection = TRUE;

		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		
		int row = 0;
		int col = 0;
		if (!HitTest(ptCursor, row, col))
		{
			return;
		}
		
		m_selectionStart.x = m_selectionEnd.x = col;
		m_selectionStart.y = m_selectionEnd.y = row;
	}

	void CTerminal::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		//KillTimer(TIMER_SELECTION_ID);
	}

	void CTerminal::OnLButtonUp(UINT nFlags, CPoint point)
	{
		__super::OnLButtonUp(nFlags, point);

		m_bSelection = FALSE;

		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);

		int row = 0;
		int col = 0;
		if (!HitTest(ptCursor, row, col))
		{
			return;
		}

		m_selectionEnd.x = col;
		m_selectionEnd.y = row;
		Invalidate(TRUE);
	}

	void CTerminal::OnMouseMove(UINT nFlags, CPoint point)
	{
		__super::OnMouseMove(nFlags, point);

		if (!m_bSelection)
		{
			return;
		}

		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);

		int row = 0;
		int col = 0;
		if (!HitTest(ptCursor, row, col))
		{
			return;
		}

		m_selectionEnd.x = col;
		m_selectionEnd.y = row;
		Invalidate(TRUE);
	}

	BOOL CTerminal::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		BOOL bRet = __super::OnMouseWheel(nFlags, zDelta, pt);

		SCROLLINFO info;
		ZeroMemory(&info, sizeof(SCROLLINFO));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;

		GetScrollInfo(SB_VERT, &info);

		CRect rcClient;
		GetClientRect(&rcClient);

		int nOffset = (int)zDelta * (int)info.nPage / rcClient.Height();
		int nPos = m_vscrollPos - nOffset;
		
		ScrollToLine(nPos);

		return bRet;
	}

	UINT CTerminal::OnGetDlgCode()
	{
		UINT lRet = __super::OnGetDlgCode();
		lRet |= DLGC_WANTALLKEYS;
		return lRet;
	}

	void CTerminal::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		SCROLLINFO info;
		ZeroMemory(&info, sizeof(SCROLLINFO));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;

		GetScrollInfo(SB_VERT, &info);

		int iPos = m_vscrollPos;

		switch (nSBCode) {
		case SB_LINEDOWN:
			iPos = iPos + 1;
			break;
		case SB_LINEUP:
			iPos = iPos - 1;
			break;
		case SB_PAGEUP:
			iPos = iPos - info.nPage;
			break;
		case SB_PAGEDOWN:
			iPos = iPos + info.nPage;
			break;
		case SB_TOP:
			iPos = info.nMin;
			break;
		case SB_BOTTOM:
			iPos = info.nMax;
			break;
		case SB_THUMBTRACK:
			iPos = info.nTrackPos;
			break;
		break;
		case SB_ENDSCROLL:
			return;
		default:
			break;
		}
		
		ScrollToLine(iPos);
	}

	BOOL CTerminal::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
		//__super::OnSetCursor(pWnd, nHitTest, message);
		HCURSOR hCursor = LoadCursor(NULL, IDC_IBEAM);
		::SetCursor(hCursor);

		return TRUE;
	}

	void CTerminal::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (m_pDelegate && m_pDelegate->OnKeyDown(nChar, nRepCnt, nFlags))
		{
			return;
		}

		SCROLLINFO info;
		ZeroMemory(&info, sizeof(SCROLLINFO));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;

		GetScrollInfo(SB_VERT, &info);

		int iPos = m_vscrollPos;
		switch (nChar)
		{
		case VK_UP:
		{
			iPos = iPos - 1;
			break;
		}
		case VK_DOWN:
		{
			iPos = iPos + 1;
			break;
		}
		case VK_PRIOR:
		{
			iPos = iPos - info.nPage;
			break;
		}
		case VK_NEXT:
		{
			iPos = iPos + info.nPage;
			break;
		}
		default:
			return __super::OnKeyDown(nChar, nRepCnt, nFlags);
			break;
		}

		ScrollToLine(iPos);
	}

	void CTerminal::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (m_pDelegate && m_pDelegate->OnChar(nChar, nRepCnt, nFlags))
		{
			return;
		}
	}

	void CTerminal::GetVisibleLines(int& nFirst, int& nLast)
	{
		nFirst = m_vscrollPos;

		int nLines = GetVisibleRowsCount();
		//int nMaxLine = GetMaxLines();
		int nMaxLine = m_pDelegate->GetTotalLines();

		nLast = std::min<int>(m_vscrollPos + nLines, nMaxLine);
		nLast -= 1;
	}

	int CTerminal::GetVisibleRowsCount(BOOL bExtend/* = TRUE*/)
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		int nRowHeight = m_pPaintManager->GetTextProcessor()->GetRowHeight();;
		int nLines = rcClient.Height() / nRowHeight;
		if (bExtend && rcClient.Height() % nRowHeight != 0)
		{
			nLines += 1;
		}

		return nLines;
	}

	int CTerminal::GetMaxLines()
	{
		int nLines = GetVisibleRowsCount(FALSE);
		int nTotalLines = 0;

		if (m_pDelegate)
		{
			nTotalLines = m_pDelegate->GetTotalLines();
		}

		return std::max<int>(nLines, nTotalLines);
	}

	void CTerminal::SelectWord(CPoint point)
	{
		int nRow = 0, nCol = 0;

		if (!RowColFromPoint(point, &nRow, &nCol))
			return;

		
	}

	BOOL CTerminal::RowColFromPoint(CPoint pt, int *pRow, int *pCol, int *pDispRow /*= NULL*/, int *pDispCol /*= NULL*/)
	{
		return TRUE;
	}

	void CTerminal::RecalcScrollBars()
	{
		if (!m_hWnd || !::IsWindow(m_hWnd))
			return;

		int nLines = GetVisibleRowsCount(FALSE);
		int nMaxLine = GetMaxLines();

		if (nMaxLine <= nLines)
		{
			EnableScrollBarCtrl(SB_VERT, FALSE);
			return;
		}

		// create scroll bar info
		SCROLLINFO info;
		ZeroMemory(&info, sizeof(SCROLLINFO));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;

		//GetScrollInfo(SB_VERT, &info);

		info.nMin = 0;
		info.nMax = nMaxLine - 1;
		info.nPage = nLines;
		info.nPos = m_vscrollPos;

		SetScrollInfo(SB_VERT, &info);
		EnableScrollBarCtrl(SB_VERT, TRUE);
	}

	BOOL CTerminal::GetSelectionRegion(CPoint& ptStart, CPoint& ptEnd)
	{
		if (m_selectionStart.y < m_selectionEnd.y)
		{
			ptStart = m_selectionStart;
			ptEnd = m_selectionEnd;
		}
		else if (m_selectionStart.y == m_selectionEnd.y)
		{
			if (m_selectionStart.x == m_selectionEnd.x)
			{
				return FALSE;
			}

			ptStart.y = ptEnd.y = m_selectionStart.y;
			ptStart.x = std::min<int>(m_selectionStart.x, m_selectionEnd.x);
			ptEnd.x = std::max<int>(m_selectionStart.x, m_selectionEnd.x);
		}
		else
		{
			ptEnd = m_selectionStart;
			ptStart = m_selectionEnd;
		}

		return TRUE;
	}

	std::vector<CRect> CTerminal::GetSelectionRegion(CDC* pDC)
	{
		std::vector<CRect> vecRegion;

		CPoint ptStart, ptEnd;
		if (!GetSelectionRegion(ptStart, ptEnd))
		{
			return vecRegion;
		}
		
		int nFirstRow = ptStart.y;
		int nLastRow = ptEnd.y;
		int nMidRows = std::max<int>(0, ptEnd.y - ptStart.y - 1);

		if (nFirstRow == nLastRow)
		{
			vecRegion.push_back(GetVisibleRect(pDC, ptStart.y, ptStart.x, ptEnd.x));
		}
		else
		{
			vecRegion.push_back(GetVisibleRect(pDC, nFirstRow, ptStart.x, -1));
			vecRegion.push_back(GetRowVisibleRect(nFirstRow + 1, nMidRows));
			vecRegion.push_back(GetVisibleRect(pDC, nLastRow, 0, ptEnd.x));
		}

		return vecRegion;
	}

	CRect CTerminal::GetCursorRect(CDC* pDC)
	{
		return GetVisibleRect(pDC, m_curPos.y, m_curPos.x);
	}

	BOOL CTerminal::HitTest(const CPoint& pt, int& rnRow, int& rnCol)
	{
		CDrawTextProcessor* pTextProcessor = m_pPaintManager->GetTextProcessor();
		CFontDC fontDC(pTextProcessor->GetDC(), m_pPaintManager->GetFont());

		if (!pTextProcessor->HitTestRow(pt.y, rnRow))
		{
			return FALSE;
		}

		rnRow += m_vscrollPos;

		CString strLine = m_pDelegate->GetLineText(rnRow);
		if (!pTextProcessor->HitTestCol(strLine, pt.x, rnCol))
		{
			return FALSE;
		}

		return TRUE;
	}

	CRect CTerminal::GetRowVisibleRect(int nRow, int nRowCount/* = 1*/)
	{
		CDrawTextProcessor* pTextProcessor = m_pPaintManager->GetTextProcessor();
		CRect rcText = pTextProcessor->GetTextRect();

		int nFirstRow = std::max<int>(0, nRow - m_vscrollPos);
		int nLastRow = std::max<int>(0, nRow - m_vscrollPos + nRowCount);
		int nRowHeight = pTextProcessor->GetRowHeight();

		CRect rc;
		rc.left = rcText.left;
		rc.top = rcText.top + nFirstRow * nRowHeight;
		rc.right = rcText.right;
		rc.bottom = rcText.top + nLastRow * nRowHeight;
		rc.bottom = std::min<int>(rc.bottom, rcText.bottom);

		return rc;
	}

	CRect CTerminal::GetVisibleRect(CDC* pDC, int nRow, int nCol)
	{
		CDrawTextProcessor* pTextProcessor = m_pPaintManager->GetTextProcessor();

		CString strLine = m_pDelegate->GetLineText(nRow);

		CRect rc = GetRowVisibleRect(nRow);
		rc.left = pTextProcessor->GetColPosX(strLine, nCol);
		rc.right = pTextProcessor->GetColPosX(strLine, nCol + 1);
		
		return rc;
	}

	CRect CTerminal::GetVisibleRect(CDC* pDC, int nRow, int nCol1, int nCol2)
	{
		if (nCol1 == nCol2)
		{
			return CRect();
		}
		CDrawTextProcessor* pTextProcessor = m_pPaintManager->GetTextProcessor();

		CString strLine = m_pDelegate->GetLineText(nRow);

		CRect rc = GetRowVisibleRect(nRow);
		rc.left = pTextProcessor->GetColPosX(strLine, nCol1);
		rc.right = pTextProcessor->GetColPosX(strLine, nCol2);

		return rc;
	}

}

