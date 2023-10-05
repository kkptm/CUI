#include "DateTime.h"
#include <sstream>
#include <iomanip>
#include <ctime>

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 6328)
DateTime::DateTime(UINT64 _dateData)
{
	this->dateData = _dateData;
	SYSTEMTIME st;
	FileTimeToSystemTime((FILETIME*)&dateData, &st);
	Year = st.wYear;
	Month = st.wMonth;
	DayOfWeek = st.wDayOfWeek;
	Day = st.wDay;
	Hour = st.wHour;
	Minute = st.wMinute;
	Second = st.wSecond;
	Millisecond = st.wMilliseconds;
}
DateTime::DateTime(FILETIME _dateData)
{
	this->dateData = *(UINT64*)&_dateData;
	SYSTEMTIME st = {};
	FileTimeToSystemTime(&_dateData, &st);
	Year = st.wYear;
	Month = st.wMonth;
	DayOfWeek = st.wDayOfWeek;
	Day = st.wDay;
	Hour = st.wHour;
	Minute = st.wMinute;
	Second = st.wSecond;
	Millisecond = st.wMilliseconds;

}
DateTime DateTime::Now()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::FromFileTime(UINT64 fileTime)
{
	return DateTime(fileTime);
}
DateTime DateTime::Parse(std::string str)
{
	std::tm tm = {};
	std::istringstream ss(str);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	if (ss.fail())
	{
		throw std::runtime_error("Failed to parse date time string");
	}
	DateTime dt = 0;
	dt.dateData = mktime(&tm);
	dt.Year = tm.tm_year + 1900;
	dt.Month = tm.tm_mon + 1;
	dt.DayOfWeek = tm.tm_wday;
	dt.Day = tm.tm_mday;
	dt.Hour = tm.tm_hour;
	dt.Minute = tm.tm_min;
	dt.Second = tm.tm_sec;
	dt.Millisecond = tm.tm_sec * 1000;
	return dt;
}
bool DateTime::IsLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}
DateTime DateTime::AddYears(int years)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData;
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	st.wYear += years;
	SystemTimeToFileTime(&st, &ft);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::AddMonths(int months)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData;
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	st.wMonth += months;
	SystemTimeToFileTime(&st, &ft);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::AddDays(int days)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData + (days * 864000000000);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::AddHours(int hours)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData + (hours * 36000000000);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::AddMinutes(int minutes)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData + (minutes * 600000000);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::AddSeconds(int seconds)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData + (seconds * 10000000);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::AddMilliseconds(int milliseconds)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData + (milliseconds * 10000);
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::AddTicks(int ticks)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData + ticks;
	return DateTime(*(UINT64*)&ft);
}
DateTime DateTime::Add(int years, int months, int days, int hours, int minutes, int seconds, int milliseconds)
{
	FILETIME ft;
	*(UINT64*)&ft = dateData;
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	st.wYear += years;
	st.wMonth += months;
	st.wDay += days;
	st.wHour += hours;
	st.wMinute += minutes;
	st.wSecond += seconds;
	st.wMilliseconds += milliseconds;
	SystemTimeToFileTime(&st, &ft);
	return DateTime(*(UINT64*)&ft);
}
std::string DateTime::ToString()
{
	char buffer[256];
	sprintf_s(buffer, "%04d-%02d-%02d %02d:%02d:%02d.%03d", Year, Month, Day, Hour, Minute, Second, Millisecond);
	return std::string(buffer);
}

bool DateTime::operator== (const DateTime & other)
{
		return dateData == other.dateData;
}
bool DateTime::operator!=(const DateTime& other)
{
	return dateData != other.dateData;
}
bool DateTime::operator>(const DateTime& other)
{
	return dateData > other.dateData;
}
bool DateTime::operator<(const DateTime& other)
{
	return dateData < other.dateData;
}
bool DateTime::operator>=(const DateTime& other)
{
	return dateData >= other.dateData;
}
bool DateTime::operator<=(const DateTime& other)
{
	return dateData <= other.dateData;
}