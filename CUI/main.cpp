#include "TestWindow.h"
#include "GUI//ProcessSelectWindow.h"
#include "Utils/StopWatch.h"
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

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
#include "Utils/ProcessOperator.h"
int main()
{
    
    //支持最新版64位PC微信远程call发送消息(3.9.5.81-3.9.9.27)
    //注入都不需要 爽歪歪啊
    ProcessOperator cep = ProcessOperator(Process::GetProcessesByName("WeChat.exe")[0].Id);
    if (cep.Open())
    {
        std::string str = StringHelper::Format("GetTickCount = %d", GetTickCount());
        auto title = cep.AllocateString("Hello");
        auto msg = cep.AllocateString(str);
        auto v = cep.CallRemote(
            (ULONG64)MessageBoxA, 
            { NULL, msg, title,MB_YESNO,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}//可以传递任意数量的参数(INT_MAX / 8)
        );
        cep.FreeMemory(title, 0x10);
        cep.FreeMemory(msg, 0x10);
        printf("%d\n",v);
        ULONG64 found = cep.FindPattern("WeChatWin.dll", "E8 ? ? ? ? ? 89 5C 24 38 89 5C 24 30");
        if (found)
        {
            found = cep.FindPattern(found, "E8 ? ? ? ? 48 8D", 64, 7);
            if (found)
            {
                BYTE code[0x40];
                cep.Read(found, code, 0x40);

                ULONG64 SendTextMsg = NULL;
                ULONG64 FreeChatMsg = NULL;

                SendTextMsg = (decltype(SendTextMsg))RELOC(code, 1);
                if (code[7] == 0x8D) {
                    FreeChatMsg = (decltype(FreeChatMsg))RELOC(code, 13);
                }
                else {
                    FreeChatMsg = (decltype(FreeChatMsg))RELOC(code, 10);
                }
                SendTextMsg += (found - ULONG64(code));
                FreeChatMsg += (found - ULONG64(code));
                const wchar_t* wx_id = L"wxid_ikvxmdbzbeu922";
                const wchar_t* wx_msg = L"测试消息";//因为我这里代码文件保存的就是UTF-8格式，所以不用编码转换否则需要编码转换:Convert::string_to_wstring(Convert::AnsiToUtf8(L"测试消息"));
                ULONG64 ProcessHeap = cep.CallRemote((ULONG64)GetProcessHeap, 0);
                //远程构造id和消息结构
                ULONG64 wxid = cep.CallRemote((ULONG64)HeapAlloc, ProcessHeap, 0, wcslen(wx_id) * 2);
                cep.Write(wxid, (void*)wx_id, wcslen(wx_id) * 2);
                ULONG64 msg = cep.CallRemote((ULONG64)HeapAlloc, ProcessHeap, 0, wcslen(wx_msg) * 2);
                cep.Write(msg, (void*)wx_msg, wcslen(wx_msg) * 2);

                ULONG64 wxid_struct = cep.CallRemote((ULONG64)HeapAlloc, ProcessHeap, 0, 0x20);
                ULONG64 msg_struct = cep.CallRemote((ULONG64)HeapAlloc, ProcessHeap, 0, 0x20);

                cep.Write(wxid_struct, wxid);
                cep.Write(wxid_struct + 8 + 0, wcslen(wx_id));
                cep.Write(wxid_struct + 8 + 4, wcslen(wx_id));

                cep.Write(msg_struct, msg);
                cep.Write(msg_struct + 8 + 0, wcslen(wx_msg));
                cep.Write(msg_struct + 8 + 4, wcslen(wx_msg));
                //远程构造消息缓存
                ULONG64 chat_msg = cep.AllocateMemory(0x1000);
                //call
                cep.CallRemote(SendTextMsg, chat_msg, wxid_struct, msg_struct, 0, 1, 1, 0, 0);
                cep.CallRemote(FreeChatMsg, chat_msg);
                //释放远程内存
                cep.FreeMemory(chat_msg);
                cep.CallRemote((ULONG64)HeapFree, ProcessHeap, 0, wxid_struct);
                cep.CallRemote((ULONG64)HeapFree, ProcessHeap, 0, msg_struct);
                cep.CallRemote((ULONG64)HeapFree, ProcessHeap, 0, wxid);
                cep.CallRemote((ULONG64)HeapFree, ProcessHeap, 0, msg);

                printf("%p\n", ProcessHeap);
            }
        }
    }
    //Clipboard::Clear();
    //Clipboard::SetFiles({ R"(C:\Windows\Win.ini)" ,R"(C:\Windows\system.ini)" });
    //auto clipBoardFiles= Clipboard::GetFiles();
    //Clipboard::SetImage(Graphics::CopyFromScreen(0,0,500,500));
    //HBITMAP clipBoardBitmap = Clipboard::GetImage();
    //Clipboard::SetText("123AAA456BBB789CCC");
    //auto clipBoardText = Clipboard::GetText();
    //StringBuilder sb;
    //for (int i = 0; i < 10; i++)
    //{
    //    sb.AppendLine(i);
    //}
    //sb << false << "\n" << 123456789 << "\n";
    //sb.AppendLine(true);
    //sb.AppendLine('A');
    //sb.AppendLine(L'B');
    //sb.AppendLine("AAA");
    //sb.AppendLine(L"AAA");
    //sb.AppendLine(0.789);
    //sb.AppendLine(0.789f);
    //sb.Lenght -= 5;
    //auto str = sb.ToWString();
    ////序列化测试
    //{
    //    pack tmp =
    //    {
    //        "NAME",
    //        100,
    //        512.0f,
    //        5000000,
    //        subpack
    //        {
    //            "SUB_NAME",
    //            200,
    //            1024.0f,
    //            80000000000,
    //       }
    //    };
    //    DataPack pk = tmp;
    //    pk["child_test"] = tmp;
    //    pk["child_test"]["child_child_test"] = tmp;
    //    auto bytes = pk.GetBytes();
    //    auto unpackedobj = DataPack(bytes);
    //    auto unpack = unpackedobj.convert<pack>();
    //    auto unpack_child = unpackedobj["child_test"].convert<pack>();
    //    auto unpack_child_child = unpackedobj["child_test"]["child_child_test"].convert<pack>();
    //    Sleep(0);
    //}
    int index = 0;
    TestWindow* mainForm = new TestWindow();
    //mainForm->ShowInTaskBar = false;
    mainForm->Text = L"test window";
    mainForm->Icon = (HICON)::LoadImageA(NULL, "app.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    //顶层渲染测试
    mainForm->OnPaint += [](Form* sender)
        {
            //auto pbx = ((TestWindow*)sender)->picturebox1;
            //if (pbx->IsVisual)
            //{

            //    auto size = sender->Render->GetScreenSize(0);
            //    auto hBitmap = sender->Render->CopyFromScreen(0, 0, size.cx, size.cy);
            //    auto bmp = sender->Render->CreateBitmap(hBitmap);

            //    auto loc = pbx->AbsLocation;
            //    sender->Render->DrawBitmap(bmp, 0, 0, size.cx, size.cy, loc.x, loc.y, pbx->Width, pbx->Height, 1.0f);
            //    bmp->Release(); 
            //    DeleteObject(hBitmap);
            //}
            sender->Render->FillRect(0, 0, sender->Size.cx * 0.2f, sender->Size.cy, D2D1_COLOR_F{ 1.0f,0.0f,0.0f,0.1f });
            sender->Render->FillRect(sender->Size.cx * 0.2f, 0, sender->Size.cx * 0.2f, sender->Size.cy, D2D1_COLOR_F{ 0.0f,1.0f,0.0f,0.1f });
            sender->Render->FillRect(sender->Size.cx * 0.4f, 0, sender->Size.cx * 0.2f, sender->Size.cy, D2D1_COLOR_F{ 0.0f,0.0f,1.0f,0.1f });
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
        mainForm->progressbar1->PercentageValue = ((GetTick() % 5000000) / 5000000.0f);
        bar->SetValue(mainForm->progressbar1->PercentageValue * 100, 100);
        Form::DoEvent();
    }
    return 0;
}