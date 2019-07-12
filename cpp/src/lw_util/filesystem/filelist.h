
#pragma once

#include "lw_util.h"
#include <string>
#include <list>
#include <vector>

namespace lw_util {

	class FileList
	{
	public:
		FileList();
		~FileList();

	public:
		void GetAll(std::list<std::string>& files);

		void Append(const std::string& path);
		bool Remove(const std::string& path, bool removefile = true);
		bool Remove(const std::vector<std::string>& files, bool removefile = true, bool skip_error = true);

		bool Clear(bool removefile = true, bool skip_error = true);

	protected:
		std::list<std::string> m_files;
	};
}

