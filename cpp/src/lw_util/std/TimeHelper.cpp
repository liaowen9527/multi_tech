#include "TimeHelper.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "PlatformConfig.h"

time_t TimeHelper::Now()
{
	return time(0);
}

time_t TimeHelper::NowMsec()
{
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();

	return (now - boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))).total_milliseconds();
}

time_t TimeHelper::GetMidnight(time_t t)
{
	auto tm1 = TimeToUtcTm(t);
	tm1.tm_hour = 0;
	tm1.tm_min = 0;
	tm1.tm_sec = 0;

	return UtcTmToTime(&tm1);
}

time_t TimeHelper::String2STLTime(const std::string& sDate)
{
	struct tm tm1;

#if TARGET_PLATFORM == PLATFORM_LINUX
	strptime(sDate.c_str(), "%Y-%m-%d %H:%M:%S", &tm1);

#else
	boost::posix_time::ptime t(boost::posix_time::time_from_string(sDate));
	tm1 = boost::posix_time::to_tm(t);
#endif

	return LocalTmToTime(&tm1);
}

tm TimeHelper::ISO8601ToTm(const std::string& sDate)
{
	struct tm time_struct;

#if TARGET_PLATFORM == PLATFORM_LINUX
#else
	std::istringstream ss(sDate);
	ss >> std::get_time(&time_struct, "%Y-%m-%dT%H:%M:%SZ");
#endif

	return time_struct;
}

std::string TimeHelper::TmToString(const tm& tm1)
{
	char str[100];
	memset(str, 0, 100);

	strftime(str, 100, "%Y-%m-%d %H:%M:%S", &tm1);
	return (std::string(str));
}

std::string TimeHelper::TmToString(const tm& tm1, int msec)
{
	char buf[32];
#if TARGET_PLATFORM == PLATFORM_LINUX
	sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
		tm1.tm_hour, tm1.tm_min, tm1.tm_sec, msec);
#else
	sprintf_s(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
		tm1.tm_hour, tm1.tm_min, tm1.tm_sec, msec);
#endif

	return buf;
}

tm TimeHelper::TimeToLocalTm(time_t t)
{
	struct tm tm1;
#if TARGET_PLATFORM == PLATFORM_LINUX
	localtime_r(&t, &tm1);
#else
	localtime_s(&tm1, &t);
#endif

	return tm1;
}

tm TimeHelper::TimeToUtcTm(time_t t)
{
	struct tm tm1;
#if TARGET_PLATFORM == PLATFORM_LINUX
	gmtime_r(&t, &tm1);
#else
	gmtime_s(&tm1, &t);
#endif

	return tm1;
}

time_t TimeHelper::LocalTmToTime(tm* tm1)
{
	return mktime(tm1);
}

time_t TimeHelper::UtcTmToTime(tm* tm1)
{
#if TARGET_PLATFORM == PLATFORM_LINUX
	return timegm(tm1);
#else
	return _mkgmtime(tm1);
#endif
}

std::string TimeHelper::TimeToString(time_t t)
{
	auto tm1 = TimeToLocalTm(t);
	return TmToString(tm1);

}

std::string TimeHelper::TimeToUtcString(time_t t)
{
	auto tm1 = TimeToUtcTm(t);
	return TmToString(tm1);
}

std::string TimeHelper::MsecTimeToString(time_t tMsec)
{
	time_t sec = tMsec / 1000;
	int msec = tMsec % 1000;

	auto tm1 = TimeToLocalTm(sec);
	return TmToString(tm1, msec);
}

std::string TimeHelper::MsecTimeToUtcString(time_t tMsec)
{
	time_t sec = tMsec / 1000;
	int msec = tMsec % 1000;

	auto tm1 = TimeToUtcTm(sec);
	return TmToString(tm1, msec);
}