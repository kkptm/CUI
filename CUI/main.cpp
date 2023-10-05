#include "TestWindow.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main()
{
    TestWindow* mainForm = new TestWindow();
    mainForm->Text = L"this is a test window";
    mainForm->Icon = (HICON)::LoadImageA(NULL, "app.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    mainForm->Show();
    int i = 0;
    mainForm->TopMost = true;
    while (1)
    {
        Form::DoEvent();
        mainForm->progressbar1->PercentageValue = ((GetTick() % 5000000) / 5000000.0f);
        if(mainForm->progressbar1->IsVisual)
            mainForm->ControlChanged = true;
    }
	return 0;
}