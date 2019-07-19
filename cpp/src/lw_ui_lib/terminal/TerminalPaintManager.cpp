#include "TerminalPaintManager.h"
#include "Terminal.h"

namespace lw_ui
{

	TerminalPaintManager::TerminalPaintManager(CTerminal* pTerminal/* = NULL*/)
	{
		m_pTerminal = pTerminal;
		if (!m_pTerminal)
		{
			abort();
		}

		InitStyle();
	}

	TerminalPaintManager::~TerminalPaintManager()
	{

	}

	CDrawTextProcessor* TerminalPaintManager::GetTextProcessor()
	{
		return &m_textProcessor;
	}

	CFont* TerminalPaintManager::GetFont()
	{
		return &m_font;
	}

	void TerminalPaintManager::SetFont(const LOGFONT& logfont)
	{
		if (m_font.m_hObject)
		{
			m_font.DeleteObject();
		}
		m_font.CreateFontIndirect(&logfont);
	}

	void TerminalPaintManager::SetFont(const CString& strFontName, int nFontSize)
	{
		LOGFONT logfont;
		::ZeroMemory(&logfont, sizeof(LOGFONT));

		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfWeight = FW_NORMAL;
		logfont.lfHeight = nFontSize;
		_tcscpy(logfont.lfFaceName, (LPCTSTR)strFontName);

		SetFont(logfont);
	}

	BOOL TerminalPaintManager::IsShowLineNumber()
	{
		return m_bShowNumber;
	}

	void TerminalPaintManager::ShowLineNumber(BOOL bShow)
	{
		m_bShowNumber = bShow;
	}

	void TerminalPaintManager::BeginPaint(CDC* pDC, const CRect& rcClient)
	{
		m_rcClient = rcClient;

		CDrawTextProcessor* pTextProcessor = GetTextProcessor();
		pTextProcessor->SetDC(pDC);
		pTextProcessor->RecalcRowHeight(pDC, GetFont());

		TerminalDelegate* pDelegate = m_pTerminal->GetDelegate();
		if (pDelegate)
		{
			int nRowCount = pDelegate->GetTotalLines();
			int nRowNumLen = (nRowCount > 9 ? (int)log10l(nRowCount) : 0) + 1;

			const TEXTMETRIC& tm = pTextProcessor->GetTextMetrics();
			m_nLineNumLength = (nRowNumLen + 1) * (tm.tmAveCharWidth) + 2;
		}

		CRect rcText = rcClient;
		rcText.left += m_nLineNumLength + 4;
		pTextProcessor->SetTextRect(rcText);
	}

	void TerminalPaintManager::DrawBackground(CDC* pDC)
	{
		pDC->FillSolidRect(m_rcClient, m_clrBack);

		CRect rcNumber = m_rcClient;
		rcNumber.right = rcNumber.top + m_nLineNumLength;
		pDC->FillSolidRect(rcNumber, m_clrBackNumer);
	}

	void TerminalPaintManager::DrawSelection(CDC* pDC, const CRect& rc)
	{
		pDC->FillSolidRect(rc, m_clrSelection);
	}

	void TerminalPaintManager::DrawCursor(CDC* pDC, const CRect& rc, BOOL bActive)
	{
		if (bActive)
		{
			pDC->FillSolidRect(rc, m_clrCursor);
		}
		else
		{
			pDC->SetTextColor(m_clrCursor);
			pDC->Rectangle(rc);
		}
	}

	void TerminalPaintManager::DrawLines(CDC* pDC, int nFirstLine, int nLastLine)
	{
		TerminalDelegate* pDelegate = m_pTerminal->GetDelegate();
		if (NULL == pDelegate)
		{
			return;
		}

		CRect rcText = m_textProcessor.GetTextRect();
		int nRowHeight = m_textProcessor.GetRowHeight();
		for (int nLine = nFirstLine; nLine <= nLastLine; ++nLine)
		{
			int nX = rcText.left;
			int nY = rcText.top + (nLine - nFirstLine) * nRowHeight;

			if (IsShowLineNumber())
			{
				DrawLineNum(pDC, nY, nLine);
			}

			std::vector<TextBlock> vecBlock;
			pDelegate->GetLine(nLine, vecBlock);

			for (const TextBlock& block : vecBlock)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(block.clrForeground);
				//pDC->SetBkColor(m_clrBack);

				const CString& strContent = block.strContext;
				int nTextLen = block.strContext.GetLength();

				pDC->TextOut(nX, nY, strContent, nTextLen);

				CSize sz = pDC->GetTextExtent(strContent);
				nX += sz.cx;
			}
		}

	}

	void TerminalPaintManager::DrawLineNum(CDC* pDC, int nY, int number)
	{
		int nRowHeight = m_textProcessor.GetRowHeight();

		CRect rcNum;
		rcNum.left = m_rcClient.left;
		rcNum.right = rcNum.right + m_nLineNumLength - 2;
		rcNum.top = nY;
		rcNum.bottom = rcNum.top + nRowHeight;

		CString strNum;
		strNum.Format(_T("%d"), number + 1);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clrNumer);
		pDC->DrawText(strNum, &rcNum, DT_VCENTER | DT_RIGHT);
	}

	void TerminalPaintManager::InitStyle()
	{
		SetFont(_T("Courier New"), -14);

		m_clrBack = RGB(0, 0, 0);
		m_clrForeground = RGB(255, 255, 255);
		m_clrCursor = RGB(0, 255, 0);
		m_clrSelection = RGB(229, 229, 229);
		m_clrBackNumer = RGB(52, 52, 52);
		m_clrNumer = RGB(255, 255, 255);

		m_bShowNumber = TRUE;
		m_nLineNumLength = 0;
	}

}

