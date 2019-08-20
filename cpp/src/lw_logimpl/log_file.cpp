#include "log_file.h"
#include <stdarg.h>
#include <sstream>
#include "filesystem/process.h"
#include "log4cpp/Category.hh"

namespace lw_util {

	Logfile::Logfile()
	{

	}

	Logfile::~Logfile()
	{

	}

	void Logfile::Log(LogEntityPtr ptr)
	{
		std::stringstream ss;
		ss << "[" << ptr->m_pid << ":" << ptr->m_tid << "] [" << ptr->m_function << "] " << ptr->m_message;

		log4cpp::Category& logger = log4cpp::Category::getInstance(ptr->m_inst);

		switch (ptr->m_level)
		{
		case LOG_DEBUG:
			logger.debug(ss.str());
			break;
		case LOG_INFO:
			logger.info(ss.str());
			break;
		case LOG_NOTICE:
			logger.notice(ss.str());
			break;
		case LOG_WARN:
			logger.warn(ss.str());
			break;
		case LOG_ERROR:
			logger.error(ss.str());
			break;
		case LOG_CRIT:
			logger.crit(ss.str());
			break;
		case LOG_ALERT:
			logger.alert(ss.str());
			break;
		case LOG_EMERG:
			logger.emerg(ss.str());
			break;
		default:
			break;
		}
	}

}
