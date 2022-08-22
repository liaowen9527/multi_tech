
#pragma once

#include "lw_util.h"
#include <string>
#include <vector>
#include <list>

class LWUtil_API LWFileSystem
{
public:
	LWFileSystem(const std::string& strRootFolder);
	virtual ~LWFileSystem();

public:
	std::string GetRootFolder(bool bCreate = true);
	void SetRootFolder(const std::string& strFolder);

public:
	std::string GetFolder(const std::string& strFolder, bool bCreate = true);
	std::string GetSubFolder(const std::string& strFolderName, bool bCreate = true);
	std::string GetSubFile(const std::string& strFullName);

	static std::string MakeAbsolute(const std::string& path);

public:
	static bool MakeDir(const std::string& strPath);
	static bool Exist(const std::string& strPath, bool bLog = false);

	static bool DeleteFile(const std::string& strFile);
	static bool DeleteFolder(const std::string& strFolder);

	static bool CopyFolder(const std::string& strFolderSrc, const std::string& strFolderDest);

	static bool MoveAll(const std::string& strFolderSrc, const std::string& strFolderDest);
	static bool MoveFile(const std::string& strFilepath, const std::string& strFolder);
	static bool MoveFile2(const std::string& strFilepath, const std::string& strDestFilePath);

	static bool AllFiles(const std::string& strFolder, std::vector<std::string>& vecFolder, std::vector<std::string>& vecFile);

protected:
	std::string m_strRootFolder;
};