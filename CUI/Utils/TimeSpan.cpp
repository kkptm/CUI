#include "TimeSpan.h"
TimeSpan::TimeSpan(__int64 ticks)
{
	_ticks = ticks;
}
TimeSpan::TimeSpan(int hours, int minutes, int seconds)
{
	__int64 num = (__int64)hours * 3600L + (__int64)minutes * 60L + seconds;
	_ticks = num * 10000000;
}
TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
{
	__int64 num = ((__int64)days * 3600L * 24 + (__int64)hours * 3600L + (__int64)minutes * 60L + seconds) * 1000 + 0;
	_ticks = num * 10000;
}
TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
{
	__int64 num = ((__int64)days * 3600L * 24 + (__int64)hours * 3600L + (__int64)minutes * 60L + seconds) * 1000 + milliseconds;
	_ticks = num * 10000;
}
void TimeSpan::Add(TimeSpan ts)
{
	_ticks += ts._ticks;
}
int TimeSpan::Days()
{
	return (int)(Hours() / 24);
}
int TimeSpan::Hours()
{
	return (int)(Minutes() / 60);
}
int TimeSpan::Milliseconds()
{
	return (int)(_ticks / 10000);
}
int TimeSpan::Minutes()
{
	return (int)(Seconds() / 60);
}
int TimeSpan::Seconds()
{
	return (int)(Milliseconds() / 1000);
}

int TimeSpan::Ticks()
{
	return (int)_ticks;
}
bool TimeSpan::operator==(TimeSpan ts)
{
	return this->_ticks == ts._ticks;
}
bool TimeSpan::operator!=(TimeSpan ts)
{
	return this->_ticks != ts._ticks;
}
bool TimeSpan::operator<(TimeSpan ts)
{
	return this->_ticks < ts._ticks;
}
bool TimeSpan::operator<=(TimeSpan ts)
{
	return this->_ticks <= ts._ticks;
}
bool TimeSpan::operator>(TimeSpan ts)
{
	return this->_ticks > ts._ticks;
}
bool TimeSpan::operator>=(TimeSpan ts)
{
	return this->_ticks >= ts._ticks;
}
TimeSpan TimeSpan::operator+(TimeSpan ts)
{
	TimeSpan result = {0};
	result._ticks = this->_ticks + ts._ticks;
	return result;
}
TimeSpan TimeSpan::operator-(TimeSpan ts)
{
	TimeSpan result = { 0 };
	result._ticks = this->_ticks - ts._ticks;
	return result;
}
TimeSpan TimeSpan::operator+=(TimeSpan ts)
{
	this->_ticks += ts._ticks;
	return *this;
}
TimeSpan TimeSpan::operator-=(TimeSpan ts)
{
	this->_ticks -= ts._ticks;
	return *this;
}