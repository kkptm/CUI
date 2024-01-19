#pragma once
#include "defines.h"
#include <string>
class DateTime
{
public:
	UINT64 dateData;
	UINT16 Year;
	UINT16 Month;
	UINT16 DayOfWeek;
	UINT16 Day;
	UINT16 Hour;
	UINT16 Minute;
	UINT16 Second;
	UINT16 Millisecond;
	DateTime(UINT64 _dateData);
	DateTime(FILETIME _dateData);
	static DateTime Now();
	static bool IsLeapYear(int year);
	static DateTime FromFileTime(UINT64 fileTime);
	static DateTime Parse(std::string str);
	DateTime AddYears(int years);
	DateTime AddMonths(int months);
	DateTime AddDays(int days);
	DateTime AddHours(int hours);
	DateTime AddMinutes(int minutes);
	DateTime AddSeconds(int seconds);
	DateTime AddMilliseconds(int milliseconds);
	DateTime AddTicks(int ticks);
	DateTime Add(int years, int months, int days, int hours, int minutes, int seconds, int milliseconds);
	std::string ToString();
	bool operator==(const DateTime& other);
	bool operator!=(const DateTime& other);
	bool operator>(const DateTime& other);
	bool operator<(const DateTime& other);
	bool operator>=(const DateTime& other);
	bool operator<=(const DateTime& other);


};

