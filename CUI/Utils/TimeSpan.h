#pragma once
#include <Windows.h>
struct TimeSpan
{
	INT64 _ticks;
	TimeSpan(INT64 ticks);
	TimeSpan(int hours, int minutes, int seconds);
	TimeSpan(int days, int hours, int minutes, int seconds);
	TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);
	void Add(TimeSpan ts);
	int Days();
	int Hours();
	int Milliseconds();
	int Minutes();
	int Seconds();
	double TotalDays();
	double TotalHours();
	double TotalMilliseconds();
	double TotalMinutes();
	double TotalSeconds();
};
