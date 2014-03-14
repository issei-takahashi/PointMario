#pragma once

#include "Singleton.h"

/*------------------- Class Timer ---------------------------

�ړI�F���ݎ����i�~���b�j�̌v��
�����F���m�X�e�[�g

--------------------------------------------------------*/

class Timer : public Singleton<Timer>
{
public:

	class TimeStamp
	{
	public:
		TimeStamp()
			:year(0),month(0),day(0),hour(0),minute(0),sec(0)
		{}
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int sec;
	};

	Timer::TimeStamp getTimeStamp();
	string getTimeStampString();
	clock_t getms();
};