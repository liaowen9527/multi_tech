#include "LWFileSystem.h"
#include <fstream>
#include <sstream>
#include "boost/filesystem.hpp"
#include "log/LWLog.h"


LWFileSystem::LWFileSystem(const std::string& strRootFolder)
{
	m_strRootFolder = MakeAbsolute(strRootFolder);
}

LWFileSystem::~LWFileSystem()
{

}

std::string LWFileSystem::GetRootFolder(bool bCreate /*= true*/)
{
	return GetFolder(m_strRootFolder, bCreate);
}

void LWFileSystem::SetRootFolder(const std::string& strFolder)
{
	m_strRootFolder = strFolder;
}

std::string LWFileSystem::GetFolder(const std::string& strFolder, bool bCreate /*= true*/)
{
	std::string strFolderAbsolute = MakeAbsolute(strFolder);
	if (strFolderAbsolute.empty())
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "failed to get absolute path, path=%s", strFolder.c_str());
		return "";
	}

	do
	{
		boost::system::error_code err;
		if (!bCreate || boost::filesystem::exists(strFolderAbsolute, err))
		{
			break;
		}

		if (boost::filesystem::create_directory(strFolderAbsolute, err))
		{
#ifdef NB_LINUX
			boost::filesystem::permissions(strFolderAbsolute,
				boost::filesystem::perms::owner_all | boost::filesystem::perms::group_exe | boost::filesystem::perms::group_read | boost::filesystem::perms::others_exe | boost::filesystem::perms::others_read,
				err);
#endif
			break;
		}

		LW_LOG(InstFilesystem, LOG_WARN, "failed to create directory, path=%s; reason=%s", strFolderAbsolute.c_str(), err.message().c_str());
	} while (0);

	return *strFolderAbsolute.rbegin() == '/' ? strFolderAbsolute : strFolderAbsolute + "/";
}

std::string LWFileSystem::GetSubFolder(const std::string& strFolderName, bool bCreate /*= true*/)
{
	std::string strRoot = GetRootFolder(bCreate);
	return GetFolder(strRoot + strFolderName);
}

std::string LWFileSystem::GetSubFile(const std::string& strFullName)
{
	return GetRootFolder() + strFullName;
}

std::string LWFileSystem::MakeAbsolute(const std::string& path)
{
	std::string ret;
	try
	{
		boost::filesystem::path filePath(path);
		//boost::filesystem::canonical: path must be existed.
		//boost::filesystem::path tempPath = boost::filesystem::canonical(filePath, ec);
		if (!filePath.is_absolute())
		{
			filePath = boost::filesystem::absolute(filePath);
		}

		assert(boost::filesystem::path("foo/").lexically_normal() == "foo/");
		assert(boost::filesystem::path("foo//").lexically_normal() == "foo/");
		assert(boost::filesystem::path("foo/./bar/..").lexically_normal() == "foo");
		assert(boost::filesystem::path("foo/.///bar/../").lexically_normal() == "foo/.");

		boost::filesystem::path tempPath = filePath.lexically_normal();
		ret = tempPath.generic_string();

		if (ret.substr(ret.length() - 1, 1) == ".")
		{
			ret = ret.substr(0, ret.length() - 1);
		}
		else if (ret.substr(ret.length() - 1, 1) != "/")
		{
			ret = ret + "/";
		}
	}
	catch (...)
	{
		ret = path;
		LW_LOG(InstFilesystem, LOG_ERROR, "unkown error, path:%s", path.c_str());
	}

	return ret;
}


bool LWFileSystem::MakeDir(const std::string& strPath)
{
	std::string strFolder;
	boost::system::error_code ec;
	if (boost::filesystem::is_directory(strPath, ec))
	{
		strFolder = strPath;
	}
	else
	{
		boost::filesystem::path ph(strPath);
		strFolder = ph.parent_path().string();
	}

	if (boost::filesystem::exists(strFolder, ec))
	{
		return true;
	}

	if (boost::filesystem::create_directories(strFolder, ec))
	{
		boost::filesystem::permissions(strFolder,
			boost::filesystem::perms::owner_all | boost::filesystem::perms::group_exe | boost::filesystem::perms::group_read | boost::filesystem::perms::others_exe | boost::filesystem::perms::others_read);

		return true;
	}

	LW_LOG(InstFilesystem, LOG_ERROR, "failed to create folder:%s, err:%s", strFolder.c_str(), ec.message().c_str());
	return false;
}

bool LWFileSystem::Exist(const std::string& strPath, bool bLog/* = false*/)
{
	boost::system::error_code ec;
	if (boost::filesystem::exists(strPath, ec))
	{
		return true;
	}

	if (bLog)
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "the filepath is not exist:%s, err:%s", strPath.c_str(), ec.message().c_str());
	}

	return false;
}

bool LWFileSystem::DeleteFile(const std::string& strFile)
{
	boost::system::error_code error;
	boost::filesystem::remove(strFile, error);
	if (error)
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "failed to delete file. error: %s", error.message().c_str());
		return false;
	}

	return true;
}

bool LWFileSystem::DeleteFolder(const std::string& strFolder)
{
	boost::system::error_code error;
	if (!boost::filesystem::exists(strFolder, error))
	{
		return true;
	}

	boost::filesystem::remove_all(strFolder, error);
	if (error)
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "failed to delete folder %s. error: %s", strFolder.c_str(), error.message().c_str());
		return false;
	}

	return true;
}

bool LWFileSystem::CopyFolder(const std::string& strFolderSrc, const std::string& strFolderDest)
{
	boost::system::error_code ec;
	if (!boost::filesystem::exists(strFolderSrc, ec))
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "can not find the srcfolder: %s", strFolderSrc.c_str());
		return false;
	}

	if (!boost::filesystem::exists(strFolderDest, ec))
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "can not find the destfolder: %s", strFolderDest.c_str());
		return false;
	}

	const boost::filesystem::path src = strFolderSrc;
	const boost::filesystem::path dst = strFolderDest;
	std::string strRootFolder = "";

	boost::filesystem::recursive_directory_iterator itEnd;
	for (boost::filesystem::recursive_directory_iterator itr(strFolderSrc.c_str()); itr != itEnd; ++itr)
	{
		const boost::filesystem::path temp = itr->path();
		boost::filesystem::path name = temp.filename();
		std::string strFullTemp = temp.string();
		if (strRootFolder.empty())
		{
			strRootFolder = strFullTemp.substr(0, strFullTemp.length() - name.string().length());
		}

		std::string strRelative = strFullTemp.substr(strRootFolder.length());
		boost::filesystem::path newPath = strFolderDest + strRelative;

		if (boost::filesystem::is_directory(temp, ec))
		{
			bool bRet = boost::filesystem::create_directory(newPath, ec);
			continue;
		}

		if (ec)
		{
			LW_LOG(InstFilesystem, LOG_ERROR, "failed to check if it is a folder: %s", strFullTemp.c_str());
			return false;
		}

		boost::filesystem::copy_file(temp, newPath, boost::filesystem::copy_option::overwrite_if_exists, ec);
		if (ec)
		{
			LW_LOG(InstFilesystem, LOG_ERROR, "failed to copy file: %s", strFullTemp.c_str());
			return false;
		}
	}

	return true;
}

bool LWFileSystem::MoveAll(const std::string& strFolderSrc, const std::string& strFolderDest)
{
	boost::system::error_code error;
	if (!boost::filesystem::exists(strFolderSrc, error))
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "can not find the srcfolder: %s", strFolderSrc.c_str());
		return false;
	}

	if (!boost::filesystem::exists(strFolderDest, error))
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "can not find the destfolder: %s", strFolderDest.c_str());
		return false;
	}

	std::vector<std::string> vecFolder;
	std::vector<std::string> vecFile;
	if (!AllFiles(strFolderSrc, vecFolder, vecFile))
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "can not get files from folder %s", strFolderSrc.c_str());
		return false;
	}

	for (const std::string& strFilename : vecFile)
	{
		std::string strFilepath = strFolderSrc + "/" + strFilename;
		if (!MoveFile(strFilepath, strFolderDest))
		{
			return false;
		}
	}

	for (const std::string& strSubFolder : vecFolder)
	{
		std::string strOldFolderPath = strFolderSrc + "/" + strSubFolder;
		std::string strNewFolderPath = strFolderDest + "/" + strSubFolder;

		boost::system::error_code err;
		if (!boost::filesystem::exists(strNewFolderPath, err))
		{
			if (!boost::filesystem::create_directory(strNewFolderPath, err))
			{
				LW_LOG(InstFilesystem, LOG_ERROR, "failed to create folder %s", strNewFolderPath.c_str());
				return false;
			}
		}

		if (!MoveAll(strOldFolderPath, strNewFolderPath))
		{
			return false;
		}
	}

	return true;
}

bool LWFileSystem::MoveFile(const std::string& strFilepath, const std::string& strFolder)
{
	boost::system::error_code error;
	if (!boost::filesystem::exists(strFolder, error))
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "can not find the folder: %s", strFolder.c_str());
		return false;
	}

	boost::filesystem::path path(strFilepath);
	std::string strDest = strFolder + "/" + path.filename().string();
	return MoveFile2(strFilepath, strDest);
}

bool LWFileSystem::MoveFile2(const std::string& strFilepath, const std::string& strDestFilePath)
{
	boost::system::error_code error;
	if (!boost::filesystem::exists(strFilepath, error))
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "can not find the file: %s", strFilepath.c_str());
		return false;
	}

	if (boost::filesystem::exists(strDestFilePath, error))
	{
		if (!boost::filesystem::remove(strDestFilePath, error))
		{
			LW_LOG(InstFilesystem, LOG_ERROR, "failed to delete the file: %s", strDestFilePath.c_str());
			return false;
		}
	}

	boost::filesystem::rename(strFilepath, strDestFilePath, error);
	if (error)
	{
		LW_LOG(InstFilesystem, LOG_ERROR, "failed to rename file from %s, to %s", strFilepath.c_str(), strDestFilePath.c_str());
		return false;
	}

	return true;
}

bool LWFileSystem::AllFiles(const std::string& strFolder, std::vector<std::string>& vecFolder, std::vector<std::string>& vecFile)
{
	boost::filesystem::directory_iterator itEnd;
	for (boost::filesystem::directory_iterator itr(strFolder.c_str()); itr != itEnd; ++itr)
	{
		boost::filesystem::path name = itr->path().filename();
		if (boost::filesystem::is_directory(*itr))
		{
			vecFolder.push_back(name.string());
		}
		else
		{
			vecFile.push_back(name.string());
		}
	}

	return true;
}
