#pragma once
#include <windows.h>
#include <iostream>
#include <functional>
#include <condition_variable>

class Thread {
private:
    HANDLE hThread;
    DWORD threadID;
    LPVOID m_func;
    LPVOID threadParam1;
    LPVOID threadParam2;
    LPVOID threadParam3;
    LPVOID threadParam4;
    LPVOID threadParam5;
    LPVOID threadParam6;
    LPVOID threadParam7;
    LPVOID threadParam8;
    bool isRunning;
    ULONG64 result;
public:
    Thread(LPVOID func, 
        LPVOID param1 = NULL, 
        LPVOID param2 = NULL,
        LPVOID param3 = NULL,
        LPVOID param4 = NULL, 
        LPVOID param5 = NULL, 
        LPVOID param6 = NULL,
        LPVOID param7 = NULL, 
        LPVOID param8 = NULL );
    ~Thread();
    void Start();
    void Join();
    DWORD GetThreadID();
    HANDLE GetHandle();
    bool IsRunning();
    ULONG64 Result();
};