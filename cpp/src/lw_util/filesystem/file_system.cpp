#include "file_system.h"
#include <fstream>
#include <sstream>
#include "boost/filesystem.hpp"
#include "log/logger_define.h"

namespace lw_util {

	FileSystem::FileSystem(const std::string& strRootFolder)
	{
		m_strRootFolder = MakeAbsolute(strRootFolder);
	}

	FileSystem::~FileSystem()
	{

	}

	bool FileSystem::Exists(const std::string& strPath)
	{
		return boost::filesystem::exists(strPath);
	}

	bool FileSystem::Delete(const std::string& strPath)
	{
		boost::system::error_code ec;
		boost::filesystem::remove_all(strPath, ec);

		if (ec)
		{
			return false;
		}

		return true;
	}

	bool FileSystem::CreateFolder(const std::string& strPath)
	{
		boost::system::error_code err;
		if (boost::filesystem::create_directory(strPath, err))
		{
			return true;
		}

		EasyLog(InstFilesystem, LOG_WARN, "failed to create directory, path=%s; reason=%s", strPath.c_str(), err.message().c_str());
		return false;
	}

	std::string FileSystem::GetRootFolder(bool bCreate /*= true*/)
	{
		return GetFolder(m_strRootFolder, bCreate);
	}

	void FileSystem::SetRootFolder(const std::string& strFolder)
	{
		m_strRootFolder = strFolder;
	}

	std::string FileSystem::GetFolder(const std::string& strFolder, bool bCreate /*= true*/)
	{
		std::string strFolderAbsolute = MakeAbsolute(strFolder);
		if (strFolderAbsolute.empty())
		{
			EasyLog(InstFilesystem, LOG_ERROR, "failed to get absolute path, path=%s", strFolder.c_str());
			return "";
		}

		do
		{
			if (!bCreate || boost::filesystem::exists(strFolderAbsolute))
			{
				break;
			}

			boost::system::error_code err;
			if (boost::filesystem::create_directory(strFolderAbsolute, err))
			{
				break;
			}

			EasyLog(InstFilesystem, LOG_WARN, "failed to create directory, path=%s; reason=%s", strFolderAbsolute.c_str(), err.message().c_str());
		} while (0);

		return *strFolderAbsolute.rbegin() == '/' ? strFolderAbsolute : strFolderAbsolute + "/";
	}

	std::string FileSystem::GetSubFolder(const std::string& strFolderName, bool bCreate /*= true*/)
	{
		std::string strRoot = GetRootFolder(bCreate);
		return GetFolder(strRoot + strFolderName);
	}

	std::string FileSystem::GetSubFile(const std::string& strFullName)
	{
		return GetRootFolder() + strFullName;
	}

	std::string FileSystem::MakeAbsolute(const std::string& path)
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
			EasyLog(InstFilesystem, LOG_ERROR, "unkown error, path:%s", path.c_str());
		}

		return ret;
	}


}


