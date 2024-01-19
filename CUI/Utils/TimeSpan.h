#pragma once
#include "defines.h"
struct TimeSpan
{
	__int64 _ticks;
	TimeSpan(__int64 ticks);
	TimeSpan(int hours, int minutes, int seconds);
	TimeSpan(int days, int hours, int minutes, int seconds);
	TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);
	void Add(TimeSpan ts);
	int Days();
	int Hours();
	int Milliseconds();
	int Minutes();
	int Seconds();
    int Ticks();
	bool operator==(TimeSpan ts);
	bool operator!=(TimeSpan ts);
	bool operator<(TimeSpan ts);
	bool operator<=(TimeSpan ts);
	bool operator>(TimeSpan ts);
	bool operator>=(TimeSpan ts);
	TimeSpan operator+(TimeSpan ts);
	TimeSpan operator-(TimeSpan ts);
	TimeSpan operator+=(TimeSpan ts);
	TimeSpan operator-=(TimeSpan ts);
};
