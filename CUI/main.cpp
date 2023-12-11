#include "TestWindow.h"
#include "GUI//ProcessSelectWindow.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main()
{

    TestWindow* mainForm = new TestWindow();
    //mainForm->ShowInTaskBar = false;
    mainForm->Text = L"test window";
    mainForm->Icon = (HICON)::LoadImageA(NULL, "app.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    mainForm->Show();
    Taskbar* bar = new Taskbar(mainForm->Handle);
    while (1)
    {
        Form::DoEvent();
        mainForm->progressbar1->PercentageValue = ((GetTick() % 5000000) / 5000000.0f);
        if(mainForm->progressbar1->IsVisual)
            mainForm->ControlChanged = true;
        bar->SetValue(mainForm->progressbar1->PercentageValue * 100, 100);
    }
    return 0;
}
