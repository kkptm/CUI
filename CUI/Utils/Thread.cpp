#include "Thread.h"

Thread::Thread(LPVOID func,
    LPVOID param1,
    LPVOID param2,
    LPVOID param3,
    LPVOID param4,
    LPVOID param5,
    LPVOID param6,
    LPVOID param7,
    LPVOID param8) {
        m_func = func;
        threadParam1 = param1;
        threadParam2 = param2;
        threadParam3 = param3;
        threadParam4 = param4;
        threadParam5 = param5;
        threadParam6 = param6;
        threadParam7 = param7;
        threadParam8 = param8;
        isRunning = false;
    }
Thread::~Thread() {
    CloseHandle(hThread);
}
void Thread::Start() {
    isRunning = true;
    hThread = CreateThread(NULL, 0, [](LPVOID param)->DWORD
        {
            Thread* inf = (Thread*)param;
            inf->isRunning = true;
            inf->result = 0;
            ULONG64(*Func)(LPVOID, LPVOID, LPVOID, LPVOID, LPVOID, LPVOID, LPVOID, LPVOID);
            Func = (decltype(Func))inf->m_func;
            inf->result = Func(inf->threadParam1, inf->threadParam2, inf->threadParam3, inf->threadParam4, inf->threadParam5, inf->threadParam6, inf->threadParam7, inf->threadParam8);
            inf->isRunning = false;
            return 0;
        }, this, 0, &threadID);
    if (hThread == NULL) {
        std::cerr << "CreateThread failed: " << GetLastError() << std::endl;
    }
}

void Thread::Join() {
    if (isRunning) {
        WaitForSingleObject(hThread, INFINITE);
        isRunning = false;
    }
}
DWORD Thread::GetThreadID() {
    return threadID;
}

HANDLE Thread::GetHandle()
{
    return hThread;
}
bool Thread::IsRunning() {
    return isRunning;
}

ULONG64 Thread::Result()
{
    return result;
}