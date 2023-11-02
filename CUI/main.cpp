#include "TestWindow.h"
#include "GUI//ProcessSelectWindow.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main()
{
    TestWindow* mainForm = new TestWindow();
    
    ProcessSelectWindow* psfm = new ProcessSelectWindow();//第一个模态窗口
    ProcessSelectWindow* psfm1 = new ProcessSelectWindow();//第二个模态窗口
    psfm->ShowDialog();//这个函数会阻塞到窗口关闭
    psfm1->ShowDialog();
    //前面两个窗口关闭后，才会执行到这里
    //上面之所以先创建窗口再显示,而不是创建一个显示一个,是因为当窗口关闭时会清理全局窗口表,当没有任何窗口存在时,程序会自动退出

    auto h= psfm->SelectedProcessId;
    auto h1 = psfm1->SelectedProcessId;
    delete psfm;
    delete psfm1;

    mainForm->Text = L"test window";
    mainForm->Icon = (HICON)::LoadImageA(NULL, "app.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    mainForm->Show();
    while (1)
    {
        Form::DoEvent();
        mainForm->progressbar1->PercentageValue = ((GetTick() % 5000000) / 5000000.0f);
        if(mainForm->progressbar1->IsVisual)
            mainForm->ControlChanged = true;
    }
    return 0;
}
