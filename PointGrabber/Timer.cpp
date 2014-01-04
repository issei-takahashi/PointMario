#include "Timer.h"
#include "utils.h"

Timer::TimeStamp Timer::getTimeStamp()
{
	time_t     current;
	struct tm  *local;
	char  wday_name[][4] = { "��", "��", "��", "��", "��", "��", "�y" };

	time(&current);                     /* ���݂̎������擾 */
	local = localtime(&current);        /* �n�����̍\���̂ɕϊ� */
	//printf("%4d�N%02d��%02d��(%s)", local->tm_year + 1900       /* �N */
	//	, local->tm_mon + 1           /* �� */
	//	, local->tm_mday              /* �� */
	//	, wday_name[local->tm_wday]   /* �j�� */
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