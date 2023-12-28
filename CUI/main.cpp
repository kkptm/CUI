#include "TestWindow.h"
#include "GUI//ProcessSelectWindow.h"
#include "Utils/StopWatch.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
struct subpack
{
    const char sname[16];
    int sv1;
    float sv2;
    __int64 sv3;
};
struct pack
{
    const char name[16];
    int v1;
    float v2;
    __int64 v3;
    subpack sub;
};
int main()
{
    Clipboard::Clear();
    Clipboard::SetFiles({ R"(C:\Windows\Win.ini)" ,R"(C:\Windows\system.ini)" });
    auto clipBoardFiles= Clipboard::GetFiles();
    Clipboard::SetImage(Graphics::CopyFromScreen(0,0,500,500));
    HBITMAP clipBoardBitmap = Clipboard::GetImage();
    Clipboard::SetText("123AAA456BBB789CCC");
    auto clipBoardText = Clipboard::GetText();
    StringBuilder sb;
    for (int i = 0; i < 10; i++)
    {
        sb.AppendLine(i);
    }
    sb << false << "\n" << 123456789 << "\n";
    sb.AppendLine(true);
    sb.AppendLine('A');
    sb.AppendLine(L'B');
    sb.AppendLine("AAA");
    sb.AppendLine(L"AAA");
    sb.AppendLine(0.789);
    sb.AppendLine(0.789f);
    sb.Lenght -= 5;
    auto str = sb.ToWString();
    //序列化测试
    {
        pack tmp =
        {
            "NAME",
            100,
            512.0f,
            5000000,
            subpack
            {
                "SUB_NAME",
                200,
                1024.0f,
                80000000000,
           }
        };
        DataPack pk = tmp;
        pk["child_test"] = tmp;
        pk["child_test"]["child_child_test"] = tmp;
        auto bytes = pk.GetBytes();
        auto unpackedobj = DataPack(bytes);
        auto unpack = unpackedobj.convert<pack>();
        auto unpack_child = unpackedobj["child_test"].convert<pack>();
        auto unpack_child_child = unpackedobj["child_test"]["child_child_test"].convert<pack>();
        Sleep(0);
    }
    int index = 0;
    TestWindow* mainForm = new TestWindow();
    //mainForm->ShowInTaskBar = false;
    mainForm->Text = L"test window";
    mainForm->Icon = (HICON)::LoadImageA(NULL, "app.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    //顶层渲染测试
    mainForm->OnPaint += [](Form* sender)
        {
            auto pbx = ((TestWindow*)sender)->picturebox1;
            if (pbx->IsVisual)
            {

                auto size = sender->Render->GetScreenSize(0);
                auto hBitmap = sender->Render->CopyFromScreen(0, 0, size.cx, size.cy);
                auto bmp = sender->Render->CreateBitmap(hBitmap);

                auto loc = pbx->AbsLocation;
                sender->Render->DrawBitmap(bmp, 0, 0, size.cx, size.cy, loc.x, loc.y, pbx->Width, pbx->Height, 1.0f);
                bmp->Release(); 
                DeleteObject(hBitmap);
            }
            //sender->Render->FillRect(0, 0, sender->Size.cx * 0.2f, sender->Size.cy, D2D1_COLOR_F{ 1.0f,0.0f,0.0f,0.1f });
            //sender->Render->FillRect(sender->Size.cx * 0.2f, 0, sender->Size.cx * 0.2f, sender->Size.cy, D2D1_COLOR_F{ 0.0f,1.0f,0.0f,0.1f });
            //sender->Render->FillRect(sender->Size.cx * 0.4f, 0, sender->Size.cx * 0.2f, sender->Size.cy, D2D1_COLOR_F{ 0.0f,0.0f,1.0f,0.1f });
        };
    mainForm->Show();
    NotifyIcon notifyIcon;
    notifyIcon.InitNotifyIcon(mainForm->Handle, 1000);
    notifyIcon.SetToolTip("CUI_TestDemo");
    notifyIcon.SetIcon(mainForm->Icon);
    notifyIcon.ShowNotifyIcon();
    notifyIcon.OnNotifyIconMouseDown += [](class NotifyIcon* nicon, MouseEventArgs args)
        {
            if(args.Buttons == MouseButtons::Left)
                ShowWindow(nicon->hWnd, SW_SHOWNORMAL);
            else if (args.Buttons == MouseButtons::Right)
                ShowWindow(nicon->hWnd, SW_HIDE);
        };
    Taskbar* bar = new Taskbar(mainForm->Handle);
    while (1)
    {
        //if (mainForm->textbox2->Text.size() > 1000)
        //    mainForm->textbox2->Text = L"";
        mainForm->progressbar1->PercentageValue = ((GetTick() % 5000000) / 5000000.0f);
        bar->SetValue(mainForm->progressbar1->PercentageValue * 100, 100);
        //mainForm->textbox2->AppendLine(std::to_wstring(index++));
        //mainForm->textbox2->ScrollToEnd();-*/
        Form::DoEvent();
    }
    return 0;
}