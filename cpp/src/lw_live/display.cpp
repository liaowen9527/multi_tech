#include "display.h"
#include "std/strhelper.h"


namespace lw_live {

	Display::Display()
	{
		m_strEol = L"\r\n";
		m_rows = 20;
		m_cols = 240;
	}

	Display::~Display()
	{

	}

	void Display::SetVisibleSize(int row, int col)
	{
		m_rows = row;
		m_cols = col;
	}

	bool Display::IsAsync()
	{
		return false;
	}

	bool Display::IsNeedSleep()
	{
		return false;
	}

	void Display::CallFunc(std::function<void()> func)
	{
		if (!IsAsync())
		{
			func();
		}
	}

	void Display::OnConnected()
	{

	}

	void Display::OnDisconnected(int nErrCode, const std::wstring& err_msg)
	{

	}

	void Display::ReWriteLines(size_t nRowFrom, const std::vector<VtLine>& lines)
	{
		if (!IsAsync())
		{
			SyncReWriteLines(nRowFrom, lines);
		}
	}

	void Display::SyncReWriteLines(int nRowFrom, const std::vector<VtLine>& lines)
	{

	}

	void Display::SeekP(size_t nRow, size_t nCol)
	{
		if (!IsAsync())
		{
			SyncSeekP(nRow, nCol);
		}
	}

	void Display::SyncSeekP(size_t nRow, size_t nCol)
	{

	}

	void Display::Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::vector<std::wstring>& vecStr)
	{

	}

	void Display::Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::wstring& str)
	{

	}

	void Display::GetCursorPos(size_t& nRow, size_t& nCol)
	{

	}

	void Display::GetEndPos(size_t& nRow, size_t& nCol)
	{

	}

	void Display::TrimLines(std::vector<VtLine>& buffer, int cursorRow, int cursorCol)
	{
		std::wstring strEmpty(1, ' ');
		bool bPopBack = true;
		for (int i = buffer.size() - 1; i >= 0; --i)
		{
			VtLine& vtLine = buffer[i];
			std::wstring& str = vtLine.str;
			int nPos = str.find_first_not_of(strEmpty);
			if (nPos == std::wstring::npos)
			{
				if (i == cursorRow)
				{
					int nIndexTemp = Col2Index(vtLine, cursorCol);
					StrHelper<wchar_t>::TrimRight(str, nIndexTemp);
				}
				else
				{
					if (bPopBack)
					{
						buffer.pop_back();
					}
					else
					{
						str.clear();
					}
				}
			}
			else
			{
				bPopBack = false;
				if (i == cursorRow)
				{
					int nIndexTemp = Col2Index(vtLine, cursorCol);
					StrHelper<wchar_t>::TrimRight(str, nIndexTemp);
				}
				else
				{
					StrHelper<wchar_t>::TrimRight(str, 0);
				}
			}
		}
	}

	int Display::Col2Index(const VtLine& vtLine, int nLogicCol)
	{
		if (!vtLine.hasWchar)
		{
			return nLogicCol;
		}

		int nIndex = 0;
		int nColTemp = 0;
		const std::wstring& wstr = vtLine.str;
		while (nIndex < wstr.size())
		{
			if (nColTemp == nLogicCol)
			{
				break;
			}

			wchar_t wchar = wstr.at(nIndex);
			char ch = wchar;
			if (wchar == ch)
			{
				nColTemp += 1;
			}
			else
			{
				nColTemp += 2;
			}

			nIndex++;
		}

		return nIndex;
	}


}

