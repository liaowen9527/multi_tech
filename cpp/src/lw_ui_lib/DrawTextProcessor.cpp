#include "DrawTextProcessor.h"
#include <algorithm>

CDrawTextProcessor::CDrawTextProcessor()
{
	m_nRowHeight = 16;
	m_pDC = NULL;
}

CDrawTextProcessor::~CDrawTextProcessor()
{

}

CDC* CDrawTextProcessor::GetDC()
{
	return m_pDC;
}

void CDrawTextProcessor::SetDC(CDC* pDC)
{
	m_pDC = pDC;
}

CRect CDrawTextProcessor::GetTextRect()
{
	return m_rcTextRect;
}

void CDrawTextProcessor::SetTextRect(const CRect& rcRect)
{
	m_rcTextRect = rcRect;
}

int CDrawTextProcessor::GetRowHeight()
{
	return m_nRowHeight;
}

int CDrawTextProcessor::RecalcRowHeight(CDC* pDC, CFont* pFont)
{
	if (!pDC || !pFont)
		return m_nRowHeight;

	m_bUseOutputDC = FALSE;
	BOOL bRes = pDC->GetTextMetrics(&m_tmText);
	if (!bRes)
	{
		return m_nRowHeight;
	}

	m_nRowHeight = m_tmText.tmHeight;
	if (pDC->IsPrinting())
	{
		TEXTMETRIC tmText;
		bRes = pDC->GetOutputTextMetrics(&tmText);
		ASSERT(bRes);
		if (bRes)
		{
			if (tmText.tmHeight > m_nRowHeight)
			{
				m_nRowHeight = tmText.tmHeight;
			}

			m_bUseOutputDC = //tmText.tmHeight > m_tmText.tmHeight ||
				tmText.tmAveCharWidth > m_tmText.tmAveCharWidth ||
				tmText.tmMaxCharWidth > m_tmText.tmMaxCharWidth;

			if (m_bUseOutputDC)
				m_tmText = tmText;
		}
	}

	if (m_bUseOutputDC)
		m_nSpaceWidth = pDC->GetOutputTextExtent(_T(" "), 1).cx;
	else
		m_nSpaceWidth = pDC->GetTextExtent(_T(" "), 1).cx;

	return m_nRowHeight;
}

int CDrawTextProcessor::GetRowsCount(BOOL bWithPartlyVisible)
{
	int nHeight = m_rcTextRect.Height();
	int nRowH = std::max<int>(1, m_nRowHeight);

	if (nHeight % nRowH == 0)
	{
		return nHeight / nRowH;
	}

	int nPartly = bWithPartlyVisible ? 1 : 0;
	return nHeight / nRowH + nPartly;
}

int CDrawTextProcessor::GetTabSize()
{
	return m_nTabSize;
}

void CDrawTextProcessor::SetTabSize(int nTabSize)
{
	m_nTabSize = nTabSize;
}

int CDrawTextProcessor::GetScrollXOffset()
{
	return m_nScrollXOffset;
}

void CDrawTextProcessor::SetScrollXOffset(int nOffsetX)
{
	m_nScrollXOffset = nOffsetX;
}

const TEXTMETRIC& CDrawTextProcessor::GetTextMetrics()
{
	return m_tmText;
}

int CDrawTextProcessor::GetSpaceWidth()
{
	return m_nSpaceWidth;
}

BOOL CDrawTextProcessor::HitTestRow(int y, int& rnRow)
{
	CRect rcText = GetTextRect();
	if (y < rcText.top || y > rcText.bottom)
	{
		return FALSE;
	}

	rnRow = y / GetRowHeight();

	return TRUE;
}

BOOL CDrawTextProcessor::HitTestCol(const CString& strLine, int x, int& rnCol)
{
	CDC* pDC = GetDC();
	if (NULL == pDC)
	{
		return FALSE;
	}

	CRect rcText = GetTextRect();
	if (x < rcText.left || x > rcText.right)
	{
		return FALSE;
	}

	int nColTemp = 0;
	for (int i = 1; i < strLine.GetLength(); ++i)
	{
		int left = GetColPosX(strLine, i);
		if (left > x)
		{
			break;
		}
		nColTemp = i;
	}

	rnCol = nColTemp;

	return TRUE;
}

int CDrawTextProcessor::GetColPosX(const CString& strLine, int nCol)
{
	CDC* pDC = GetDC();
	if (NULL == pDC)
	{
		return 0;
	}

	CRect rcText = GetTextRect();

	int nOffsetX = rcText.Width();
	if (nCol >= 0)
	{
		if (strLine.GetLength() >= nCol)
		{
			CString strSub = strLine.Left(nCol);
			nOffsetX = pDC->GetTextExtent(strSub).cx;
		}
		else
		{
			nOffsetX = pDC->GetTextExtent(strLine).cx;
			nOffsetX += std::max<int>(GetSpaceWidth(), 8) * (nCol - strLine.GetLength());
		}
	}

	return rcText.left + nOffsetX;
}
