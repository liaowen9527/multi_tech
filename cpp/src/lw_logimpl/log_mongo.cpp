#include "log_mongo.h"
#include "xson/xson_builder.h"

namespace lw_util {

	LogMongo::LogMongo()
	{

	}

	LogMongo::~LogMongo()
	{

	}

	void LogMongo::SetClient(MongoDBClientPtr client)
	{
		m_clientPtr = client;
	}

	void LogMongo::Log(LogEntityPtr ptr)
	{
		MongoDBCollection collection;
		if (!m_clientPtr->GetCollection("logs", ptr->m_inst, collection))
		{
			return;
		}

		xson_builder builder;
		builder.append("lev", ptr->GetLogLevel());
		builder.append("local", ptr->m_local);
		builder.append("pid", ptr->m_pid);
		builder.append("tid", ptr->m_tid);
		builder.append("func", ptr->m_function);
		builder.append("log", ptr->m_message);
		builder.append_array("lable", [&](xson_builder& sub) {
			for (auto& keyword : ptr->m_keywords)
			{
				sub.append(keyword);
			}
		});
		
		collection.InsertOne(builder.view());
	}

}
