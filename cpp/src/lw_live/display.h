#pragma once
#include "lw_live.h"
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include "vtline.h"

namespace lw_live {

	class LWLive_API Display
	{
	public:
		Display();
		virtual ~Display();

		void SetVisibleSize(int row, int col);

	public:
		virtual bool IsAsync();
		virtual bool IsNeedSleep();

		virtual void CallFunc(std::function<void()> func);

	public:
		virtual void OnConnected();
		virtual void OnDisconnected(int nErrCode, const std::wstring& err_msg);

		virtual void ReWriteLines(size_t nRowFrom, const std::vector<VtLine>& lines);
		virtual void SyncReWriteLines(int nRowFrom, const std::vector<VtLine>& lines);

		virtual void SeekP(size_t nRow, size_t nCol);
		virtual void SyncSeekP(size_t nRow, size_t nCol);

	public:
		void Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::vector<std::wstring>& vecStr);
		void Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::wstring& str);

		void GetCursorPos(size_t& nRow, size_t& nCol);
		void GetEndPos(size_t& nRow, size_t& nCol);

		//trim blank line at the end
		//TrimRight each of line
		void TrimLines(std::vector<VtLine>& buffer, int cursorRow, int cursorCol);
		int Col2Index(const VtLine& vtLine, int nLogicCol);

	protected:
		std::wstring m_strEol;

		int m_rows;
		int m_cols;
	};

	typedef std::shared_ptr<Display> DisplayPtr;

}




