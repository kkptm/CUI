#include "TestWindow.h"
#include "GUI//ProcessSelectWindow.h"
#include "Utils/StopWatch.h"
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main()
{
    TestWindow* mainForm = new TestWindow();
    //mainForm->ShowInTaskBar = false;
    mainForm->Text = L"test window";
    mainForm->Icon = (HICON)::LoadImageA(NULL, "app.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    mainForm->Show();
    Taskbar* bar = new Taskbar(mainForm->Handle);
    while (1)
    {
        Form::DoEvent();
        mainForm->progressbar1->PercentageValue = ((GetTick() % 5000000) / 5000000.0f);
        bar->SetValue(mainForm->progressbar1->PercentageValue * 100, 100);
    }
    return 0;
}