
#pragma once

#include "lw_util.h"
#include <string>
#include <vector>
#include <list>

namespace lw_util {

	class LWUtil_API FileSystem
	{
	public:
		FileSystem(const std::string& strRootFolder);
		virtual ~FileSystem();

	public:
		static bool Exists(const std::string& strPath);

		static bool DeleteFile(const std::string& strPath);
		static bool DeleteFile(const std::string& strPath, std::string& err_msg);

		static bool DeleteFiles(const std::vector<std::string>& files, bool skip_error = true);
		static bool DeleteFiles(const std::list<std::string>& files, bool skip_error = true);

		static bool CreateFolder(const std::string& strPath);
		static bool DeleteFolder(const std::string& strPath);

	public:
		std::string GetRootFolder(bool bCreate = true);
		void SetRootFolder(const std::string& strFolder);

	public:
		std::string GetFolder(const std::string& strFolder, bool bCreate = true);
		std::string GetSubFolder(const std::string& strFolderName, bool bCreate = true);
		std::string GetSubFile(const std::string& strFullName);

		static std::string MakeAbsolute(const std::string& path);

	protected:
		std::string m_strRootFolder;
	};
}

