#include "TestWindow.h"
#include "GUI//ProcessSelectWindow.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main()
{
    TestWindow* mainForm = new TestWindow();
    
    ProcessSelectWindow* psfm = new ProcessSelectWindow();//��һ��ģ̬����
    ProcessSelectWindow* psfm1 = new ProcessSelectWindow();//�ڶ���ģ̬����
    psfm->ShowDialog();//������������������ڹر�
    psfm1->ShowDialog();
    //ǰ���������ڹرպ󣬲Ż�ִ�е�����
    //����֮�����ȴ�����������ʾ,�����Ǵ���һ����ʾһ��,����Ϊ�����ڹر�ʱ������ȫ�ִ��ڱ�,��û���κδ��ڴ���ʱ,������Զ��˳�

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
