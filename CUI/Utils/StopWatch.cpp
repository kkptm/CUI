#include "StopWatch.h"
Stopwatch* Stopwatch::StartNew()
{
    Stopwatch* st = new Stopwatch();
    st->starttime = *(__int64*)0x7FFE0348;
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
        __int64 _endtime = {};
        _endtime = *(__int64*)0x7FFE0348;
        __int64 v = _endtime - starttime;
        TimeSpan tm = TimeSpan(v);
        return tm;
    }
    else
    {
        __int64 v = endtime - starttime;
        TimeSpan tm = TimeSpan(v);
        return tm;
    }
}
void Stopwatch::Stop()
{
    this->RUNNING = false;
    this->endtime = *(__int64*)0x7FFE0348;
}
void Stopwatch::Restart()
{
    this->RUNNING = true;
    this->starttime = *(__int64*)0x7FFE0348;
}