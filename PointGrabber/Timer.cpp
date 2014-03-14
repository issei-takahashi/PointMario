#include "Timer.h"
#include "utils.h"
#include <time.h>

Timer::TimeStamp Timer::getTimeStamp()
{
	time_t     current;
	struct tm  *local;
	char  wday_name[][4] = { "日", "月", "火", "水", "木", "金", "土" };

	time(&current);                     /* 現在の時刻を取得 */
	local = localtime(&current);        /* 地方時の構造体に変換 */
	//printf("%4d年%02d月%02d日(%s)", local->tm_year + 1900       /* 年 */
	//	, local->tm_mon + 1           /* 月 */
	//	, local->tm_mday              /* 日 */
	//	, wday_name[local->tm_wday]   /* 曜日 */
	//);

	TimeStamp ret;
	ret.year   = local->tm_year + 1900 ;
	ret.month  = local->tm_mon + 1 ;
	ret.day    = local->tm_mday;
	ret.hour   = local->tm_hour;
	ret.minute = local->tm_min;
	ret.sec    = local->tm_sec;
	

	return ret;
}
string Timer::getTimeStampString()
{
	Timer::TimeStamp stamp = Timer::getTimeStamp();
	return utils::int2string(stamp.year) + "_" + utils::int2string(stamp.month) + "_" + utils::int2string(stamp.day) +
		" " + utils::int2string(stamp.hour) + ":" + utils::int2string(stamp.minute) + ":" + utils::int2string(stamp.sec);
}

clock_t Timer::getms()
{
	return clock();
}