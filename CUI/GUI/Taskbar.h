#pragma once
#include <shobjidl.h>

#pragma comment(lib, "Comctl32.lib")
class Taskbar
{
    static ITaskbarList3* pTaskbarList;
public:
    HWND Handle = NULL;
    Taskbar(HWND handle);
    void SetValue(ULONGLONG value, ULONGLONG total);
    ~Taskbar();
};

