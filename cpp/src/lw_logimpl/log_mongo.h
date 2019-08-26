
#pragma once

#include "lw_logimpl.h"
#include <string>
#include "log/logger_impl.h"
#include "mongo/MongoDBClient.h"

namespace lw_util {

	class LWLogImpl_API LogMongo : public LoggerImpl
	{
	public:
		LogMongo();
		~LogMongo();

	public:
		void SetClient(MongoDBClientPtr client);

	protected:
		virtual void Log(LogEntityPtr ptr);

	protected:
		MongoDBClientPtr m_clientPtr;
	};

}