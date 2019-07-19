#include "display_buffer_limit.h"
#include <sstream>
#include "std/string_convert.h"
#include "filesystem/file_system.h"

using namespace lw_util;

namespace lw_live {

	DisplayBufferLimit::DisplayBufferLimit()
	{
		m_nMaxLines = 30000;
		m_nOffsetLines = 0;
	}

	DisplayBufferLimit::~DisplayBufferLimit()
	{
		SaveToFile();
	}

	void DisplayBufferLimit::SetFilePath(const std::string& strFilePath)
	{
		m_strFilePath = strFilePath;
	}

	void DisplayBufferLimit::SyncReWriteLines(int nRowFrom, const std::vector<VtLine>& lines)
	{
		__super::SyncReWriteLines(nRowFrom, lines);

		LimitLines();
	}

	void DisplayBufferLimit::LimitLines()
	{
		std::wstring wstrLines;
		int nLines = __super::LimitLines(m_nMaxLines, wstrLines);
		if (nLines > 0)
		{
			m_nOffsetLines += nLines;
			SaveToFileCache(wstrLines);
		}
	}

	std::string DisplayBufferLimit::GetFileCachePath()
	{
		return m_strFilePath + ".cache";
	}

	void DisplayBufferLimit::SaveToFileCache(const std::wstring& wstr)
	{
		if (m_strFilePath.empty())
		{
			return;
		}

		std::string strFilePath = GetFileCachePath();
		if (!FileSystem::Exists(strFilePath))
		{
			m_fileCache.close();
			m_fileCache.open(strFilePath.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios::binary);
		}

		std::string strLines = StringConverter::utf_to_utf(wstr);
		m_fileCache.write(strLines.c_str(), strLines.length());

		m_fileCache.flush();
	}

	void DisplayBufferLimit::GetBufferData(std::string& str)
	{
		size_t row = 0, col = 0;
		GetEndPos(row, col);

		std::wstring wstr;
		Read(0, 0, row, col, wstr);

		str = StringConverter::utf_to_utf(wstr);
	}

	void DisplayBufferLimit::SaveToFile()
	{
		if (m_strFilePath.empty())
		{
			return;
		}

		std::fstream file;
		file.open(m_strFilePath.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios::binary);

		std::string strFilePath = GetFileCachePath();
		if (FileSystem::Exists(strFilePath))
		{
			m_fileCache.close();
			m_fileCache.open(strFilePath.c_str(), std::ios_base::in | std::ios::binary);

			std::stringstream ss;
			ss << m_fileCache.rdbuf();

			const std::string& strCache = ss.str();
			file.write(strCache.c_str(), strCache.length());

			m_fileCache.close();
			::remove(strFilePath.c_str());
		}

		std::string str;
		GetBufferData(str);

		file.write(str.c_str(), str.length());
	}

}

