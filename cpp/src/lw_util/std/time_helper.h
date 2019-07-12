
#pragma once

#include "lw_util.h"
#include <string>

namespace lw_util {

	class LWUtil_API TimeHelper
	{
	public:
		static time_t LocalTime();
		static time_t LocalUtcTime();
		static time_t LocalUtcMsecTime();

		static time_t UtcToLocal(time_t time);
		static time_t LocalToUtc(time_t time);

		static std::string ToString(time_t secTime);
		static std::string ToMsecString(time_t msecTime);
		
	};

}