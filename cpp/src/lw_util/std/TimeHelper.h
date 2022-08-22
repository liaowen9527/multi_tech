
#pragma once

#include "lw_util.h"
#include <string>

class LWUtil_API TimeHelper
{
public:
	static time_t Now();
	static time_t NowMsec();
	static time_t GetMidnight(time_t t);

	static time_t String2STLTime(const std::string& sDate);

	static std::string TmToString(const tm& tm1);
	static std::string TmToString(const tm& tm1, int msec);

	static tm TimeToLocalTm(time_t t);
	static tm TimeToUtcTm(time_t t);

	static time_t LocalTmToTime(tm* tm1);
	static time_t UtcTmToTime(tm* tm1);

	static std::string TimeToString(time_t t);
	static std::string TimeToUtcString(time_t t);

	static std::string MsecTimeToString(time_t tMsec);
	static std::string MsecTimeToUtcString(time_t tMsec);

	static tm ISO8601ToTm(const std::string& sDate);

};