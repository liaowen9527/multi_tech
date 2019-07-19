#pragma once
#include "lw_live.h"
#include "display_buffer.h"
#include <fstream>

namespace lw_live {

	class LWLive_API DisplayBufferLimit : public DisplayBuffer
	{
	public:
		DisplayBufferLimit();
		virtual ~DisplayBufferLimit();

	public:
		void SetFilePath(const std::string& strFilePath);

	protected:
		virtual void SyncReWriteLines(int nRowFrom, const std::vector<VtLine>& lines);

	protected:
		void LimitLines();

		std::string GetFileCachePath();
		void SaveToFileCache(const std::wstring& wstr);

		void GetBufferData(std::string& str);
		void SaveToFile();
	
	protected:
		std::string m_strFilePath;
		std::fstream m_fileCache;	//If the CLI data is very large, we cannot cache it all in memory.

		int m_nMaxLines;
		int m_nOffsetLines;
	};

	typedef std::shared_ptr<DisplayBufferLimit> DisplayBufferLimitPtr;

}



