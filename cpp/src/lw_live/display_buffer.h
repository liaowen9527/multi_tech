#pragma once
#include "lw_live.h"
#include <string>
#include <memory>
#include <mutex>
#include <set>
#include <vector>
#include <queue>
#include <atomic>
#include "display.h"


class LWLive_API DisplayBuffer : public Display
{
public:
	DisplayBuffer();
	virtual ~DisplayBuffer();

public:
	virtual void SyncReWriteLines(int nRowFrom, const std::vector<VtLine>& lines);

	virtual void SyncSeekP(size_t nRow, size_t nCol);

	virtual void GetCursorPos(size_t& nRow, size_t& nCol);
	virtual void GetEndPos(size_t& nRow, size_t& nCol);

	virtual void Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::vector<std::wstring>& vecStr);
	virtual void Read(size_t nRowFrom, size_t nColFrom, size_t nRowTo, size_t nColTo, std::wstring& str);

public:
	void GetModifyRows(std::vector<int>& vecRow);
	void SetModify(int nRow, bool bValue);
	void ClearModify();

	void ReadLines(std::wstring& strLines, bool bCheckCrLf = true);
	void ReadLines(size_t nRowFrom, size_t nRowTo, std::wstring& strLines, bool bCheckCrLf = true);

	void LimitLines(int nLines, std::vector<VtLine>& vecPop);
	int LimitLines(int nLines, std::wstring& strLines);

protected:
	void EnsureRow_unsafe(int nRow);
	void EnsureRowCol_unsafe(int nRow, int nCol);

	void SetLine_unsafe(int nLine, const VtLine& vtLine);

	void SetModify_unsafe(int nRow, bool bValue = true);

	int GetRealCol_unsafe(int nRow, int nLogicCol);

protected:
	int GetRealIndex(int nIndex);

protected:
	std::deque<VtLine> m_buffer;
	int m_nIndexRow;
	int m_nIndexCol;
	int m_nLogicCol;

	std::set<int> m_modify;

	std::atomic_int m_nOffsetLine;

	//std::recursive_mutex m_mutex;
	std::mutex m_mutex;
};

typedef std::shared_ptr<DisplayBuffer> STBufferDisplayPtr;


