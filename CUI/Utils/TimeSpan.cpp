#include "TimeSpan.h"
TimeSpan::TimeSpan(INT64 ticks)
{
	_ticks = ticks;
}
TimeSpan::TimeSpan(int hours, int minutes, int seconds)
{
	INT64 num = (INT64)hours * 3600L + (INT64)minutes * 60L + seconds;
	_ticks = num * 10000000;
}
TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
{
	INT64 num = ((INT64)days * 3600L * 24 + (INT64)hours * 3600L + (INT64)minutes * 60L + seconds) * 1000 + 0;
	_ticks = num * 10000;
}
TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
{
	INT64 num = ((INT64)days * 3600L * 24 + (INT64)hours * 3600L + (INT64)minutes * 60L + seconds) * 1000 + milliseconds;
	_ticks = num * 10000;
}
void TimeSpan::Add(TimeSpan ts)
{
	_ticks += ts._ticks;
}
int TimeSpan::Days()
{
	return (int)(_ticks / 864000000000L);
}
int TimeSpan::Hours()
{
	return (int)(_ticks / 36000000000L % 24);
}
int TimeSpan::Milliseconds()
{
	return (int)(_ticks / 10000 % 1000);
}
int TimeSpan::Minutes()
{
	return (int)(_ticks / 600000000 % 60);
}
int TimeSpan::Seconds()
{
	return (int)(_ticks / 10000000 % 60);
}
double TimeSpan::TotalDays()
{
	return (double)_ticks * 1.1574074074074074E-12;
}
double TimeSpan::TotalHours()
{
	return (double)_ticks * 2.7777777777777777E-11;
}
double TimeSpan::TotalMilliseconds()
{
	double num = (double)_ticks * 0.0001;
	if (num > 922337203685477.0)
	{
		return 922337203685477.0;
	}

	if (num < -922337203685477.0)
	{
		return -922337203685477.0;
	}

	return num;
}
double TimeSpan::TotalMinutes()
{
	return (double)_ticks * 1.6666666666666667E-09;
}
double TimeSpan::TotalSeconds()
{
	return (double)_ticks * 1E-07;
}