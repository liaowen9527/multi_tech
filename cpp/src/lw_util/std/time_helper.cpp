#include "time_helper.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace lw_util {

	
	time_t TimeHelper::LocalTime()
	{
		boost::posix_time::ptime pt_now = boost::posix_time::second_clock::local_time();
		struct tm tm1 = boost::posix_time::to_tm(pt_now);

		return (mktime(&tm1));
	}

	time_t TimeHelper::LocalUtcTime()
	{
		return time(0);
	}

	time_t TimeHelper::LocalUtcMsecTime()
	{
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();

		return (now - boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))).total_milliseconds();
	}

	time_t TimeHelper::UtcToLocal(time_t time)
	{
		struct tm t;
		localtime_s(&t, &time);
		return _mkgmtime(&t);
	}

	time_t TimeHelper::LocalToUtc(time_t time)
	{
		struct tm t;
		gmtime_s(&t, &time);
		return mktime(&t);
	}

	std::string TimeHelper::ToString(time_t secTime)
	{
		struct tm tm1;
		_gmtime64_s(&tm1, &secTime);

		char buf[32];
		sprintf_s(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02dZ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
			tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
		return buf;
	}

	std::string TimeHelper::ToMsecString(time_t msecTime)
	{
		struct tm tm1;
		time_t sec = msecTime / 1000;
		int msec = msecTime % 1000;
		_gmtime64_s(&tm1, &sec);

		char buf[32];
		sprintf_s(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
			tm1.tm_hour, tm1.tm_min, tm1.tm_sec, msec);

		return buf;
	}

}
