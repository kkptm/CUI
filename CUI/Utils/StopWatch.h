#pragma once
#include "defines.h"
#include "TimeSpan.h"
class Stopwatch
{
private:
    BOOLEAN RUNNING = false;
    __int64 starttime;
    __int64 endtime;
public:
    static Stopwatch* StartNew();
    bool IsRunning();
    TimeSpan Elapsed();
    void Stop();
    void Restart();
};
