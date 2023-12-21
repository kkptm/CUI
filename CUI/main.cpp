#include "TestWindow.h"
#include "GUI//ProcessSelectWindow.h"
#include "Utils/StopWatch.h"
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main()
{
    int index = 0;
    //auto str = HttpHelper::HttpGet("http://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest");
    //File::WriteAllText("D:\\ip.txt",str);
    TestWindow* mainForm = new TestWindow();
    //mainForm->ShowInTaskBar = false;
    mainForm->Text = L"test window";
    mainForm->Icon = (HICON)::LoadImageA(NULL, "app.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    mainForm->OnPaint += [](Form* sender)
        {
            sender->Render->FillRect(0, 0, sender->Size.cx*0.5f, sender->Size.cy, D2D1_COLOR_F{ 1.0f,0.0f,0.0f,0.1f });
        };
    mainForm->Show();
    Taskbar* bar = new Taskbar(mainForm->Handle);
    while (1)
    {
        if (mainForm->textbox2->Text.size() > 1000)
            mainForm->textbox2->Text = L"";
        mainForm->progressbar1->PercentageValue = ((GetTick() % 5000000) / 5000000.0f);
        bar->SetValue(mainForm->progressbar1->PercentageValue * 100, 100);
        mainForm->textbox2->AppendLine(std::to_wstring(index++));
        mainForm->textbox2->ScrollToEnd();
        Form::DoEvent();
    }
    return 0;
}