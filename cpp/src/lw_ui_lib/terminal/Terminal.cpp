#include "Terminal.h"
#include <algorithm>

namespace lw_ui
{

	CTerminal::CTerminal()
	{
		m_pPaintManager = new TerminalPaintManager(this);
		m_pDelegate = NULL;
		m_offsetLine = 0;

		m_bRecalcScroll = TRUE;
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

		SetTimer(1, 1, NULL);
	}

	BEGIN_MESSAGE_MAP(CTerminal, CStatic)
		ON_WM_PAINT()
		ON_WM_SIZE()
		ON_WM_TIMER()
		ON_WM_LBUTTONDOWN()
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
		m_curPos.x = row;
		m_curPos.y = col;
		
		if (bForceVisible)
		{
			EnsureVisible(row, col);
		}
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
		if (nLine == m_offsetLine)
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

		m_offsetLine = nLine;

		SetScrollPos(SB_VERT, m_offsetLine);

		Invalidate();
	}

	void CTerminal::ScheduleTimer(int ticks, std::function<void()> func)
	{
		time_t now = GetTickCount();
		time_t when = now + ticks;

		std::lock_guard<std::mutex> lck(m_mutex);
		m_jobs[when] = func;
	}

	void CTerminal::OnPaint()
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CPaintDC dc(this); // device context for painting
		CMemDC memDC(dc, rcClient);

		CDC* pDC = &memDC.GetDC();
		m_pPaintManager->DrawBackground(pDC, rcClient);

		m_pPaintManager->RecalcRowHeight(pDC);

		int nFirst = 0;
		int nLast = 0;
		GetVisibleLines(nFirst, nLast);

		m_pPaintManager->DrawLines(pDC, nFirst, nLast);

		if (m_bRecalcScroll)
		{
			RecalcScrollBars();
			//m_bRecalcScroll = FALSE;
		}
	}

	void CTerminal::OnSize(UINT nType, int cx, int cy)
	{
		__super::OnSize(nType, cx, cy);
	}

	void CTerminal::OnTimer(UINT_PTR nIDEvent)
	{
		__super::OnTimer(nIDEvent);

		time_t now = GetTickCount();
		std::lock_guard<std::mutex> lck(m_mutex);
		auto itrEnd = m_jobs.upper_bound(now);
		for (auto itr = m_jobs.begin(); itr != itrEnd;)
		{
			(itr->second)();
			itr = m_jobs.erase(itr);
		}
	}

	void CTerminal::OnLButtonDown(UINT nFlags, CPoint point)
	{
		__super::OnLButtonDown(nFlags, point);

		SetFocus();
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
		int nPos = m_offsetLine - nOffset;
		
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

		int iPos = m_offsetLine;

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

		int iPos = m_offsetLine;
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
		nFirst = m_offsetLine;

		int nLines = GetVisibleRowsCount();
		int nMaxLine = GetMaxLines();

		nLast = std::min<int>(m_offsetLine + nLines, nMaxLine);
		nLast -= 1;
	}

	int CTerminal::GetVisibleRowsCount(BOOL bExtend/* = TRUE*/)
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		int nRowHeight = m_pPaintManager->GetRowHeight();;
		int nLines = rcClient.Height() / nRowHeight;
		if (bExtend && rcClient.Height() % nRowHeight != 0)
		{
			nLines += 1;
		}

		return nLines;
	}

	int CTerminal::GetMaxLines()
	{
		int nLines = GetVisibleRowsCount();
		int nTotalLines = 0;

		if (m_pDelegate)
		{
			nTotalLines = m_pDelegate->GetTotalLines();
		}

		return std::max<int>(nLines, nTotalLines);
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
		info.nPos = m_offsetLine;

		SetScrollInfo(SB_VERT, &info);
		EnableScrollBarCtrl(SB_VERT, TRUE);
	}
}

