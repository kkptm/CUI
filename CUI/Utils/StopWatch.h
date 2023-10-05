#pragma once
#include "TimeSpan.h"
class Stopwatch
{
private:
    BOOLEAN RUNNING = false;
    LARGE_INTEGER starttime;
    LARGE_INTEGER endtime;
    static long long toInteger(LARGE_INTEGER const& integer);
    static LARGE_INTEGER toLargeInteger(long long value);
public:
    static Stopwatch* StartNew();
    bool IsRunning();
    TimeSpan Elapsed();
    void Stop();
    void Restart();
};
