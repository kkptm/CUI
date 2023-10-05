#include "Form.h"
#ifndef PROPERTY_CPP
#define GET_CPP(c,t,n) t c::property__get_##n() 
#define SET_CPP(c,t,n) void c::property__set_##n(t value)
#endif // !PROPERTY_CPP
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
    int len = GetWindowTextLengthW(Handle);
    wchar_t* buffer = new wchar_t[len + 1];
    GetWindowTextW(Handle, buffer, len + 1);
    std::wstring result = buffer;
    delete[] buffer;
    return result;
}
SET_CPP(Form, std::wstring, Text) {
    SetWindowTextW(Handle, value.c_str());
}
GET_CPP(Form, bool, ShowInTaskBar)
{
    return (GetWindowLong(this->Handle, GWL_EXSTYLE) & WS_EX_APPWINDOW) != 0;
}
SET_CPP(Form, bool, ShowInTaskBar)
{
    if (value)
    {
        SetWindowLong(this->Handle, GWL_EXSTYLE, GetWindowLong(this->Handle, GWL_EXSTYLE) | WS_EX_APPWINDOW);
    }
    else
    {
        SetWindowLong(this->Handle, GWL_EXSTYLE, GetWindowLong(this->Handle, GWL_EXSTYLE) & ~WS_EX_APPWINDOW);
    }
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
Form::Form(std::wstring _text, POINT _location, SIZE _size)
{
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
    this->Handle = CreateWindowW(L"CoreNativeWindow",
        _text.c_str(),
        this->Style,
        this->Location.x == 0 ? CW_USEDEFAULT : this->Location.x,
        this->Location.y == 0 ? CW_USEDEFAULT : this->Location.y,
        this->Size.cx == 0 ? CW_USEDEFAULT : this->Size.cx,
        this->Size.cy == 0 ? CW_USEDEFAULT : this->Size.cy,
        NULL,
        this->Menu,
        GetModuleHandleW(NULL),
        NULL
    );
    SetWindowLongPtrW(this->Handle, GWLP_USERDATA, (LONG_PTR)this);
    DragAcceptFiles(this->Handle, TRUE);
    Render = new Graphics(this->Handle);
    Application::Forms.Add(this->Handle, this);
}
void Form::Show()
{
    if (this->Menu) SetMenu(this->Handle, this->Menu);
    if(this->Icon) SendMessage(this->Handle, WM_SETICON, ICON_BIG, (LPARAM)this->Icon);
    SetWindowLong(this->Handle, GWL_STYLE, this->Style);
    ShowWindow(this->Handle, SW_SHOWNORMAL);
}
void Form::Close()
{
    CloseWindow(this->Handle);
}
bool Form::DoEvent()
{
    MSG msg;
    if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        return true;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    for(auto form : Application::Forms)
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
bool Form::Update()
{
    if (ControlChanged)
    {

        this->Render->BeginRender(this->BackColor);
        if (this->Image)
        {
            this->RenderImage();
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
        ControlChanged = false;
        return true;
    }
    return false;;
}
Control* Form::AddControl(Control* c)
{
    if (c->Parent)
    {
        throw "该控件已属于其他容器!";
        return NULL;
    }
    if (this->Controls.Contains(c))
    {
        return c;
    }
    this->Controls.Add(c);
    c->Parent = NULL;
    c->ParentForm = this;
    c->Render = this->Render;
    return c;
}
bool Form::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
{
    if (!this->Enable || !this->Visable) return true;
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(this->Handle, &mouse);
    Control* HitControl = NULL;
    switch (message)
    {
    case WM_MOUSEMOVE:
    {
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
            if (this->UnderMouse)this->UnderMouse->SingleUpdate();
            if (lastUnderMouse)lastUnderMouse->SingleUpdate();
        }
    }
    break;
    case WM_DROPFILES:
    case WM_MOUSEWHEEL:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_LBUTTONDBLCLK:
    {
        if (WM_LBUTTONUP == message && this->Selected)
        {
            if (this->Selected->IsVisual)
            {
                HitControl = this->Selected;
                auto location = this->Selected->AbsLocation;
                this->Selected->ProcessMessage(message, wParam, lParam, mouse.x - location.x, mouse.y - location.y);
                break;
            }
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
        se->SingleUpdate();
    }
    if(this->Render)this->Update();
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
                this->Render->DrawBitmap(this->Image,  xf, yf, size.width, size.height);
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
        return D2D1_POINT_2F{ (float)loc.x + siz.cy,(float)loc.y + siz.cy };
    }
    return { 0,0 };
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
    return result;
}
LRESULT CALLBACK Form::WINMSG_PROCESS(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Form* form = (Form*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (form && Application::Forms.ContainsKey(form->Handle))
    {
        form->ProcessMessage(message, wParam, lParam, 0, 0);
        if (message == WM_NCDESTROY)
        {
            form->OnFormClosed(form);
            Application::Forms.Remove(form->Handle);
            delete form;
            if (Application::Forms.Count() == 0)
            {
                exit(0);
            }
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}