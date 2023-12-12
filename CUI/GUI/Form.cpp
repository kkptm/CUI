#include "Form.h"
GET_CPP(Form, POINT, Location)
{
    if (this->Handle)
    {
        RECT rect;
        GetWindowRect(this->Handle, &rect);
        POINT point = { rect.left,rect.top };
        return point;
    }
    else
    {
        return this->_Location_INIT;
    }
}
SET_CPP(Form, POINT, Location)
{
    if (this->Handle)
    {
        SetWindowPos(this->Handle, NULL, value.x, value.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
    this->_Location_INIT = value;
}
GET_CPP(Form, SIZE, Size)
{
    if (this->Handle)
    {
        RECT rect;
        GetWindowRect(this->Handle, &rect);
        SIZE size = { rect.right - rect.left,rect.bottom - rect.top };
        return size;
    }
    else
    {
        return this->_Size_INTI;
    }
}
SET_CPP(Form, SIZE, Size)
{
    if (this->Handle)
    {
        SetWindowPos(this->Handle, NULL, 0, 0, value.cx, value.cy, SWP_NOMOVE | SWP_NOZORDER);
    }
    this->_Size_INTI = value;
    this->ControlChanged = true;
}

GET_CPP(Form, SIZE, ClientSize)
{
    if (this->Handle)
    {
        RECT rect;
        GetClientRect(this->Handle, &rect);
        SIZE size = { rect.right - rect.left,rect.bottom - rect.top };
        return size;
    }
    else
    {
        return this->_Size_INTI;
    }
}
GET_CPP(Form, std::wstring, Text) {
    return _text;
}
SET_CPP(Form, std::wstring, Text) {
    _text = value;
    this->ControlChanged = true;
}
GET_CPP(Form, bool, TopMost)
{
    return (GetWindowLong(this->Handle, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
}
SET_CPP(Form, bool, TopMost)
{
    if (value)
    {
        SetWindowPos(this->Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else
    {
        SetWindowPos(this->Handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}
GET_CPP(Form, bool, Enable)
{
    return IsWindowEnabled(this->Handle);
}
SET_CPP(Form, bool, Enable)
{
    EnableWindow(this->Handle, value);
}
GET_CPP(Form, bool, Visable)
{
    return IsWindowVisible(this->Handle);
}
SET_CPP(Form, bool, Visable)
{
    ShowWindow(this->Handle, value ? SW_SHOW : SW_HIDE);
}

GET_CPP(Form, bool, ShowInTaskBar)
{
    return this->_showInTaskBar;
}
SET_CPP(Form, bool, ShowInTaskBar)
{
    this->_showInTaskBar = value;
    if (this->Handle)
    {
        LONG_PTR exStyle = GetWindowLongPtr(this->Handle, GWL_EXSTYLE);

        if (value)
        {
            exStyle &= ~WS_EX_TOOLWINDOW;
            exStyle |= WS_EX_APPWINDOW;
        }
        else
        {
            exStyle |= WS_EX_TOOLWINDOW;
            exStyle &= ~WS_EX_APPWINDOW;
        }
        SetWindowLongPtr(this->Handle, GWL_EXSTYLE, exStyle);
    }
}
Form::Form(std::wstring text, POINT _location, SIZE _size)
{
    this->_text = text;
    static bool ClassInited = false;
    this->Location = _location;
    this->Size = _size;
    WNDCLASSW wndclass = { 0 };
    if (!ClassInited)
    {
        wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.lpfnWndProc = WINMSG_PROCESS;
        wndclass.hInstance = GetModuleHandleA(NULL);
        wndclass.hIcon = LoadIconW(NULL, MAKEINTRESOURCEW(32512));
        wndclass.hCursor = LoadCursorW(NULL, MAKEINTRESOURCEW(32512));
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = L"CoreNativeWindow";
        if (!RegisterClassW(&wndclass))
        {
            return;
        }
        ClassInited = true;
    }
    int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
    int desktopHeight = GetSystemMetrics(SM_CYSCREEN);
    this->Handle = CreateWindowExW(
        0L,
        L"CoreNativeWindow",
        _text.c_str(),
        WS_POPUP,
        this->Location.x == 0 ? ((int)(desktopWidth - this->Size.cx) / 2) : this->Location.x,
        this->Location.y == 0 ? ((int)(desktopHeight - this->Size.cy) / 2) : this->Location.y,
        this->Size.cx,
        this->Size.cy,
        GetCurrentActiveWindow(),
        NULL,
        GetModuleHandleW(0),
        0);
    SetWindowLongPtrW(this->Handle, GWLP_USERDATA, (LONG_PTR)this ^ 0xFFFFFFFFFFFFFFFF);
    DragAcceptFiles(this->Handle, TRUE);
    Render = new Graphics(this->Handle);
    Application::Forms.Add(this->Handle, this);

    float xtmp = this->Size.cx - (this->HeadHeight * 3);
    _minBox = (Button*)this->AddControl(new Button(L"―", xtmp, 0.0f, this->HeadHeight, this->HeadHeight));
    xtmp += this->HeadHeight;
    _maxBox = (Button*)this->AddControl(new Button(L"⬜", xtmp, 0.0f, this->HeadHeight, this->HeadHeight));
    xtmp += this->HeadHeight;
    _closeBox = (Button*)this->AddControl(new Button(L"✕", xtmp, 0.0f, this->HeadHeight, this->HeadHeight));
    _minBox->OnMouseClick += [](class Control* sender, MouseEventArgs)
        {
            ((Button*)sender)->ParentForm->Handle;
            ShowWindow(((Button*)sender)->ParentForm->Handle, SW_MINIMIZE);
        };
    _maxBox->OnMouseClick += [](class Control* sender, MouseEventArgs)
        {
            ((Button*)sender)->ParentForm->Handle;
            if (IsZoomed(((Button*)sender)->ParentForm->Handle))
            {
                ((Button*)sender)->Text = L"⬜";
                ShowWindow(((Button*)sender)->ParentForm->Handle, SW_RESTORE);
            }
            else
            {
                ((Button*)sender)->Text = L"❐";
                ShowWindow(((Button*)sender)->ParentForm->Handle, SW_MAXIMIZE);
            }
        };
    _closeBox->OnMouseClick += [](class Control* sender, MouseEventArgs)
        {
            ((Button*)sender)->ParentForm->Close();
        };
    _minBox->Boder = 0.0f; _minBox->Round = 0.0f; _minBox->BackColor = D2D1_COLOR_F{ 0.0f,0.0f,0.0f,0.0f };
    _maxBox->Boder = 0.0f; _maxBox->Round = 0.0f; _maxBox->BackColor = D2D1_COLOR_F{ 0.0f,0.0f,0.0f,0.0f };
    _closeBox->Boder = 0.0f; _closeBox->Round = 0.0f; _closeBox->BackColor = D2D1_COLOR_F{ 0.0f,0.0f,0.0f,0.0f };
}

void Form::updateHead()
{
    float xtmp = this->Size.cx - this->HeadHeight;
    if (_closeBox && CloseBox)
    {
        _closeBox->Left = xtmp; _closeBox->Size = SIZE{ this->HeadHeight ,this->HeadHeight };
        xtmp -= this->HeadHeight;
    }
    if (_maxBox && MaxBox)
    {
        _maxBox->Left = xtmp; _maxBox->Size = SIZE{ this->HeadHeight ,this->HeadHeight };
        xtmp -= this->HeadHeight;
    }
    if (_minBox && MinBox)
    {
        _minBox->Left = xtmp; _minBox->Size = SIZE{ this->HeadHeight ,this->HeadHeight };
    }
    _minBox->Visable = this->MinBox;
    _maxBox->Visable = this->MaxBox;
    _closeBox->Visable = this->CloseBox;
}
void Form::Show()
{
    updateHead();
    if (this->Icon) SendMessage(this->Handle, WM_SETICON, ICON_BIG, (LPARAM)this->Icon);
    SetWindowLong(this->Handle, GWL_STYLE, WS_POPUP);


    ShowWindow(this->Handle, SW_SHOWNORMAL);
}

void Form::ShowDialog()
{
    if (this->Icon) SendMessage(this->Handle, WM_SETICON, ICON_BIG, (LPARAM)this->Icon);
    SetWindowLong(this->Handle, GWL_STYLE, WS_POPUP);
    updateHead();
    ShowWindow(this->Handle, SW_SHOWNORMAL);
    while (IsWindow(this->Handle))
    {
        MSG msg;
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            ULONG64 form = (ULONG64)(GetWindowLongPtrW(msg.hwnd, GWLP_USERDATA) ^ 0xFFFFFFFFFFFFFFFF);
            if (msg.hwnd == this->Handle ||
                form == 0xFFFFFFFFFFFFFFFF ||
                (msg.message == WM_SYSCOMMAND && msg.wParam == SC_RESTORE))
            {
                if (IsWindow(this->Handle))
                {
                    DispatchMessageW(&msg);
                    this->Update();
                }
                else
                {
                    DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
                }
            }
            else
            {
                DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
            }
        }
        else
        {
            Sleep(1);
        }
    }
}
void Form::Close()
{
    SendMessageA(this->Handle, WM_CLOSE, 0, 0);
}
bool Form::DoEvent()
{
    MSG msg;
    if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        return true;
    }
    Sleep(1);
    for (auto form : Application::Forms)
    {
        form.second->Update();
    }
    return false;
}
bool Form::WaiteEvent()
{
    MSG msg;
    if (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        return true;
    }
    return false;
}
bool Form::Update(bool force)
{
    if (ControlChanged && IsWindow(this->Handle))
    {
        this->OnPaint(this);
        auto now = GetTickCount64();
        this->Render->BeginRender(this->BackColor);
        this->Render->Resize();
        if (this->Image)
        {
            this->RenderImage();
        }
        if (VisibleHead)
        {
            this->Render->FillRect(0, 0, this->Size.cx, this->HeadHeight, { 0.5f ,0.5f ,0.5f ,0.25f });
            float headTextTop = (this->HeadHeight - this->Render->DefaultFontObject->FontHeight) * 0.5f;
            if (headTextTop < 0.0f)
                headTextTop = 0.0f;
            this->Render->PushDrawRect(0, 0, this->Size.cx, this->HeadHeight);
            if (this->CenterTitle)
            {
                auto tSize = this->Render->DefaultFontObject->GetTextSize(this->Text);
                float textRangeWidth = this->Size.cx;
                if (tSize.width > (this->Size.cx - (this->HeadHeight * 3)))
                {
                    textRangeWidth -= this->HeadHeight * 3;
                }
                float headTextLeft = (textRangeWidth - tSize.width) * 0.5f;
                if (headTextLeft < 0.0f)
                    headTextLeft = 0.0f;
                this->Render->DrawString(this->Text, headTextLeft, headTextTop, this->ForeColor);
            }
            else
            {
                this->Render->DrawString(this->Text, 5.0f, headTextTop, this->ForeColor);
            }
            this->Render->PopDrawRect();
        }
        for (int i = 0; i < this->Controls.Count; i++)
        {
            auto c = this->Controls[i]; if (!c->Visable)continue;
            auto location = c->Location;
            if (c->Render == NULL)
            {
                c->Render = this->Render;
            }
            c->Update();
        }
        for (auto fc : this->ForeGroundControls)
        {
            fc->Update();
        }
        this->Render->EndRender();
        this->ControlChanged = false;
        return true;
    }
    return false;;
}
bool Form::ForceUpdate()
{
    this->ControlChanged = true;
    return Update(true);
}

bool Form::RemoveControl(Control* c)
{
    if (this->Controls.Contains(c))
    {
        this->Controls.Remove(c);
        this->ForeGroundControls.Remove(c);
        c->Parent = NULL;
        c->ParentForm = NULL;
        return true;
    }
    return false;
}
bool Form::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
{
    if (!this->Enable || !this->Visable) return true;
    //POINT mouse = { ((int)(short)LOWORD(lParam)),((int)(short)HIWORD(lParam)) };
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(this->Handle, &mouse);
    Control* HitControl = NULL;
    switch (message)
    {
    case WM_MOUSEMOVE:
    {
        if (offset.x || offset.y)
        {
            if (GetCapture() == this->Handle) {
                // 移动窗口
                POINT pt;
                pt.x = ((int)(short)LOWORD(lParam));
                pt.y = ((int)(short)HIWORD(lParam));
                RECT rect;
                GetWindowRect(this->Handle, &rect);
                MoveWindow(this->Handle, rect.left + pt.x - offset.x, rect.top + pt.y - offset.y,
                    rect.right - rect.left, rect.bottom - rect.top, FALSE);
            }
        }
        if (this->Selected && (GetKeyState(VK_LBUTTON) & 0x8000))
        {
            if (this->Selected->IsVisual)
            {
                HitControl = this->Selected;
                auto location = this->Selected->AbsLocation;
                this->Selected->ProcessMessage(message, wParam, lParam, mouse.x - location.x, mouse.y - location.y);
                break;
            }
        }
        auto lastUnderMouse = this->UnderMouse;
        this->UnderMouse = NULL;
        bool is_First = true;
    reExc:
        for (int i = 0; i < this->Controls.Count; i++)
        {
            auto c = this->Controls[i]; if (!c->Visable)continue;
            auto location = c->Location;
            auto size = c->ActualSize();
            if (
                mouse.x >= location.x &&
                mouse.y >= location.y &&
                mouse.x <= (location.x + size.cx) &&
                mouse.y <= (location.y + size.cy)
                )
            {
                if (is_First)
                {
                    if (c->Type() == UIClass::UI_ComboBox)
                    {
                        HitControl = c;
                        this->UnderMouse = c;
                        c->ProcessMessage(message, wParam, lParam, mouse.x - location.x, mouse.y - location.y);
                        goto ext;
                    }
                }
                else
                {
                    HitControl = c;
                    this->UnderMouse = c;
                    c->ProcessMessage(message, wParam, lParam, mouse.x - location.x, mouse.y - location.y);
                }
            }
        }
        if (is_First)
        {
            is_First = false;
            goto reExc;
        }
    ext:
        if (lastUnderMouse != this->UnderMouse)
        {
            if (this->UnderMouse)this->UnderMouse->PostRender();
            if (lastUnderMouse)lastUnderMouse->PostRender();
        }
    }
    break;
    case WM_DROPFILES:
    case WM_MOUSEWHEEL:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_LBUTTONDBLCLK:
    {
        if (WM_LBUTTONDOWN == message)
        {
            if (VisibleHead)
            {
                offset = { 0,0 };
                if (((int)(short)HIWORD(lParam)) < this->HeadHeight)
                {
                    offset.x = ((int)(short)LOWORD(lParam));
                    offset.y = ((int)(short)HIWORD(lParam));
                    SetCapture(this->Handle);
                }
            }
        }
        else if (WM_LBUTTONUP == message)
        {
            if (offset.x || offset.y)
            {
                offset = { 0,0 };
                ReleaseCapture();
            }
            if (this->Selected)
            {
                if (this->Selected->IsVisual)
                {
                    HitControl = this->Selected;
                    auto location = this->Selected->AbsLocation;
                    this->Selected->ProcessMessage(message, wParam, lParam, mouse.x - location.x, mouse.y - location.y);
                    break;
                }
            }
        }
        else if (WM_LBUTTONDBLCLK == message)
        {
            if(mouse.y<this->HeadHeight)
                this->_maxBox->OnMouseClick(this->_maxBox, MouseEventArgs{});
        }
        bool is_First = true;
    reExc1:
        for (int i = 0; i < this->Controls.Count; i++)
        {
            auto c = this->Controls[i]; if (!c->Visable)continue;
            auto location = c->Location;
            auto size = c->ActualSize();
            if (
                mouse.x >= location.x &&
                mouse.y >= location.y &&
                mouse.x <= (location.x + size.cx) &&
                mouse.y <= (location.y + size.cy)
                )
            {
                if (is_First)
                {
                    if (c->Type() == UIClass::UI_ComboBox)
                    {
                        HitControl = c;
                        c->ProcessMessage(message, wParam, lParam, mouse.x - location.x, mouse.y - location.y);
                        goto ext1;
                    }
                }
                else
                {
                    HitControl = c;
                    c->ProcessMessage(message, wParam, lParam, mouse.x - location.x, mouse.y - location.y);
                }
            }
        }
        if (is_First)
        {
            is_First = false;
            goto reExc1;
        }
    ext1:;
    }
    break;
    case WM_KEYDOWN:
    {
        if (this->Selected)
        {
            if (this->Selected->ProcessMessage(message, wParam, lParam, xof, yof))
            {
                if (this->Selected->IsVisual)
                {
                    HitControl = this->Selected;
                    KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
                    this->OnKeyDown(this, event_obj);
                }
            }
        }
        else
        {
            KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
            this->OnKeyDown(this, event_obj);
        }
    }
    break;
    case WM_KEYUP:
    {
        if (this->Selected)
        {
            if (this->Selected->ProcessMessage(message, wParam, lParam, xof, yof))
            {
                HitControl = this->Selected;
                KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
                this->OnKeyUp(this, event_obj);
            }
        }
        else
        {
            KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
            this->OnKeyUp(this, event_obj);
        }
    }
    break;
    case WM_SIZE:
    {
        RECT rec;
        GetClientRect(this->Handle, &rec);
        this->OnSizeChanged(this);
        updateHead();
        this->Render->Resize();
        this->ControlChanged = true;
    }
    break;
    case WM_MOVE:
    {
        RECT client_rectangle;
        GetClientRect(this->Handle, &client_rectangle);
        this->OnMoved(this);
    }
    break;
    case WM_PAINT:
    {
        this->OnPaint(this);
        this->ControlChanged = true;
    }
    break;
    case WM_CHAR:
    {
        if (this->Selected)
        {
            if (this->Selected->ProcessMessage(message, wParam, lParam, xof, yof))
            {
                HitControl = this->Selected;
                this->OnCharInput(this, (wchar_t)(wParam));
            }
        }
        else
        {
            HitControl = NULL;
            this->OnCharInput(this, (wchar_t)(wParam));
        }
    }
    break;
    case WM_IME_COMPOSITION:
    {
        if (this->Selected)
        {
            HitControl = this->Selected;
            this->Selected->ProcessMessage(message, wParam, lParam, xof, yof);
        }
    }
    break;
    case WM_CLOSE:
    {
        this->OnFormClosing(this);
        this->Render->~Graphics();
        return true;
    }
    break;
    case WM_COMMAND:
    {
        int id = LOWORD(wParam);
        int additionalInfo = HIWORD(wParam);
        this->OnCommand(this, id, additionalInfo);
    }
    break;
    };
    if (WM_LBUTTONDOWN == message && HitControl == NULL && this->Selected && HitControl != this->Selected)
    {
        auto se = this->Selected;
        this->Selected = NULL;
        se->PostRender();
    }
    if (IsWindow(this->Handle) && this->Render)this->Update();
    return true;
}
void Form::RenderImage()
{
    if (this->Image)
    {
        auto size = this->Image->GetSize();
        if (size.width > 0 && size.height > 0)
        {
            auto asize = this->ClientSize;
            switch (this->SizeMode)
            {
            case ImageSizeMode::Normal:
            {
                this->Render->DrawBitmap(this->Image, 0, 0, size.width, size.height);
            }
            break;
            case ImageSizeMode::CenterImage:
            {
                float xf = (asize.cx - size.width) / 2.0f;
                float yf = (asize.cy - size.height) / 2.0f;
                this->Render->DrawBitmap(this->Image, xf, yf, size.width, size.height);
            }
            break;
            case ImageSizeMode::StretchIamge:
            {
                this->Render->DrawBitmap(this->Image, 0, 0, asize.cx, asize.cy);
            }
            break;
            case ImageSizeMode::Zoom:
            {
                float xp = asize.cx / size.width;
                float yp = asize.cy / size.height;
                float tp = xp < yp ? xp : yp;
                float tw = size.width * tp;
                float th = size.height * tp;
                float xf = (asize.cx - tw) / 2.0f;
                float yf = (asize.cy - th) / 2.0f;
                this->Render->DrawBitmap(this->Image, xf, yf, tw, th);
            }
            break;
            default:
                break;
            }
        }
    }
}
D2D1_POINT_2F Form::LastChildRB()
{
    if (this->Controls.Count)
    {
        auto last = this->Controls.Last();
        auto loc = last->Location;
        auto siz = last->Size;
        return D2D1_POINT_2F{ (float)loc.x + siz.cy,(float)loc.y + siz.cy - this->HeadHeight };
    }
    return { 0,0 };
}
Control* Form::LastChild()
{
    if (this->Controls.Count)
    {
        return this->Controls.Last();
    }
    return NULL;
}
D2D1_POINT_2F Form::MaxChildRB()
{
    D2D1_POINT_2F result = { 0,0 };
    if (this->Controls.Count)
    {
        for (auto c : this->Controls)
        {
            auto last = this->Controls.Last();
            auto loc = last->Location;
            auto siz = last->ActualSize();
            auto tmp = D2D1_POINT_2F{ (float)loc.x + siz.cy,(float)loc.y + siz.cy };
            if (tmp.x > result.x)result.x = tmp.x;
            if (tmp.y > result.y)result.y = tmp.y;
        }
    }
    result.y -= this->HeadHeight;
    return result;
}
LRESULT CALLBACK Form::WINMSG_PROCESS(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Form* form = (Form*)(GetWindowLongPtrW(hWnd, GWLP_USERDATA) ^ 0xFFFFFFFFFFFFFFFF);
    if ((ULONG64)form != 0xFFFFFFFFFFFFFFFF && Application::Forms.ContainsKey(form->Handle))
    {
        form->ProcessMessage(message, wParam, lParam, 0, 0);
        if (message == WM_NCDESTROY)
        {
            form->OnFormClosed(form);
            Application::Forms.Remove(form->Handle);
            if (Application::Forms.Count() == 0)
            {
                exit(0);
            }
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}