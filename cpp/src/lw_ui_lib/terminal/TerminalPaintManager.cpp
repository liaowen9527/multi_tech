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

	CDrawTextProcessor* TerminalPaintManager::GetTextProcessor()
	{
		return &m_textProcessor;
	}

	void TerminalPaintManager::DrawBackground(CDC* pDC, const CRect& rc)
	{
		pDC->FillSolidRect(rc, m_clrBack);
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

		int nRowHeight = m_textProcessor.GetRowHeight();
		for (int nLine = nFirstLine; nLine <= nLastLine; ++nLine)
		{
			int nX = 0;
			int nY = (nLine - nFirstLine) * nRowHeight;

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

	void TerminalPaintManager::InitStyle()
	{
		SetFont(_T("Courier New"), -14);

		m_clrBack = RGB(0, 0, 0);
		m_clrForeground = RGB(255, 255, 255);
		m_clrCursor = RGB(0, 255, 0);
		m_clrSelection = RGB(229, 229, 229);

	}

}

