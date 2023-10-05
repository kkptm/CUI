#include "StopWatch.h"
long long Stopwatch::toInteger(LARGE_INTEGER const& integer)
{
#ifndef MIDL_PASS
    return integer.QuadPart;
#else
    return (static_cast<long long>(integer.HighPart) << 32) | integer.LowPart;
#endif
}
LARGE_INTEGER Stopwatch::toLargeInteger(long long value)
{
    LARGE_INTEGER result;

#ifndef MIDL_PASS
    result.QuadPart = value;
#else
    result.high_part = value & 0xFFFFFFFF00000000;
    result.low_part = value & 0xFFFFFFFF;
#endif
    return result;
}
Stopwatch* Stopwatch::StartNew()
{
    Stopwatch* st = new Stopwatch();
    QueryPerformanceCounter(&st->starttime);
    st->RUNNING = true;
    return st;
}
bool Stopwatch::IsRunning()
{
    return this->RUNNING;
}
TimeSpan Stopwatch::Elapsed()
{
    if (this->RUNNING)
    {
        LARGE_INTEGER _endtime;
        QueryPerformanceCounter(&_endtime);
        INT64 v = toInteger(_endtime) - toInteger(starttime);
        TimeSpan tm = TimeSpan(v);
        return tm;
    }
    else
    {
        INT64 v = toInteger(endtime) - toInteger(starttime);
        TimeSpan tm = TimeSpan(v);
        return tm;
    }
}
void Stopwatch::Stop()
{
    this->RUNNING = false;
    QueryPerformanceCounter(&this->endtime);
}
void Stopwatch::Restart()
{
    this->RUNNING = true;
    QueryPerformanceCounter(&this->starttime);
}