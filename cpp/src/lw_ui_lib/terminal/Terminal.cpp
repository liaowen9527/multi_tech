#include "Terminal.h"

namespace lw_ui
{

	CTerminal::CTerminal()
	{
		m_pPaintManager = new TerminalPaintManager(this);
		m_pDelegate = NULL;
		m_nDispRow = 0;
	}

	CTerminal::~CTerminal()
	{
		SetPaintManager(NULL);
	}

	BEGIN_MESSAGE_MAP(CTerminal, CWnd)
		ON_WM_PAINT()
		ON_WM_SIZE()
		ON_WM_KEYDOWN()
		ON_WM_VSCROLL()
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

	void CTerminal::OnPaint()
	{
		DWORD dwStartTime = ::GetTickCount();

		CPaintDC dc(this); // device context for painting
		CMemDC memDC(dc, this);

		CRect rcClient;
		GetClientRect(&rcClient);

		m_pPaintManager->DrawBackground(&memDC.GetDC(), rcClient);

	}

	void CTerminal::OnSize(UINT nType, int cx, int cy)
	{
		__super::OnSize(nType, cx, cy);
	}

	void CTerminal::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{

	}

	void CTerminal::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		switch (nChar)
		{
		case VK_UP:
		{
			break;
		}
		case VK_DOWN:
		{
			break;
		}
		default:
			break;
		}
	}

}

