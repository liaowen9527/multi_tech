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

		m_clrBack = RGB(0, 0, 0);
		m_clrForeground = RGB(255, 255, 255);
	}

	TerminalPaintManager::~TerminalPaintManager()
	{

	}

	void TerminalPaintManager::SetFont(const LOGFONT& logfont)
	{
		if (m_font.m_hObject)
		{
			m_font.DeleteObject();
		}
		m_font.CreateFontIndirect(&logfont);
		m_nRowHeight = 0;
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

	void TerminalPaintManager::DrawBackground(CDC* pDC, const CRect& rc)
	{
		pDC->FillSolidRect(rc, m_clrBack);
	}

	void TerminalPaintManager::DrawLines(CDC* pDC, int nFirstLine, int nLastLine)
	{
		TerminalDelegate* pDelegate = m_pTerminal->GetDelegate();
		if (NULL == pDelegate)
		{
			return;
		}

		for (int nLine = nFirstLine; nLine <= nLastLine; ++nLine)
		{
			int nX = 0;
			int nY = (nLine - nFirstLine) * m_nRowHeight;

			std::vector<TextBlock> vecBlock;
			pDelegate->GetLine(nLine, vecBlock);

			for (const TextBlock& block : vecBlock)
			{
				pDC->SetBkMode(OPAQUE);
				pDC->SetBkColor(block.clrBack);
				pDC->SetTextColor(block.clrForeground);

				const CString& strContent = block.strContext;
				int nTextLen = block.strContext.GetLength();

				pDC->TextOut(nX, nY, strContent, nTextLen);

				CSize sz = pDC->GetTextExtent(strContent);
				nX += sz.cx;
			}
		}

	}

	void TerminalPaintManager::DrawCursor(CDC* pDC)
	{

	}

	void TerminalPaintManager::InitStyle()
	{
		m_clrBack = RGB(0, 0, 0);
		m_clrForeground = RGB(255, 255, 255);

		SetFont(_T("Courier New"), 9);
	}

	void TerminalPaintManager::RecalcRowHeight(CDC* pDC)
	{
		CFont* pOldFont = pDC->SelectObject(&m_font);

		TEXTMETRIC tmText;
		BOOL bRes = pDC->GetOutputTextMetrics(&tmText);
		m_nRowHeight = tmText.tmHeight;

		pDC->SelectObject(pOldFont);
	}

	int TerminalPaintManager::GetRowHeight()
	{
		return m_nRowHeight;
	}

}

