#include "filelist.h"
#include "file_system.h"

namespace lw_util {

	FileList::FileList()
	{

	}

	FileList::~FileList()
	{

	}

	void FileList::GetAll(std::list<std::string>& files)
	{
		files = m_files;
	}

	void FileList::Append(const std::string& path)
	{
		m_files.push_back(path);
	}

	bool FileList::Remove(const std::string& path, bool removefile /*= true*/)
	{
		if (removefile)
		{
			if (!FileSystem::DeleteFile(path))
			{
				return false;
			}
		}

		m_files.remove(path);

		return true;
	}

	bool FileList::Remove(const std::vector<std::string>& files, bool removefile /*= true*/, bool skip_error /*= true*/)
	{
		if (removefile)
		{
			if (!FileSystem::DeleteFiles(files, skip_error))
			{
				return false;
			}
		}

		for (auto file : files)
		{
			m_files.remove(file);
		}

		return true;
	}

	bool FileList::Clear(bool removefile /*= true*/, bool skip_error /*= true*/)
	{
		if (removefile)
		{
			if (!FileSystem::DeleteFiles(m_files, skip_error))
			{
				return false;
			}
		}

		m_files.clear();

		return true;
	}

}


