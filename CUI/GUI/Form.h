#pragma once
#include "Control.h"
#include "Application.h"
#include "Button.h"
#include "CheckBox.h"
#include "ComboBox.h"
#include "GridView.h"
#include "Label.h"
#include "Panel.h"
#include "PasswordBox.h"
#include "PictureBox.h"
#include "ProgressBar.h"
#include "RadioBox.h"
#include "RichTextBox.h"
#include "RoundTextBox.h"
#include "Switch.h"
#include "TabControl.h"
#include "TextBox.h"
typedef Event<void(*)(void* sender, int Id,int info)> CommandEvent;
typedef Event<void(*)(void*)> FormClosingEvent;
typedef Event<void(*)(void*)> FormClosedEvent;
class Form
{
    POINT _Location_INIT;
    SIZE _Size_INTI;
    std::wstring _text;
    POINT offset;
    Button* _minBox;
    Button* _maxBox;
    Button* _closeBox;
public:
    MouseWheelEvent OnMouseWheel;
    MouseMoveEvent OnMouseMove;
    MouseUpEvent OnMouseUp;
    MouseDownEvent OnMouseDown;
    MouseDoubleClickEvent OnMouseDoubleClick;
    MouseClickEvent OnMouseClick;
    MouseEnterEvent OnMouseEnter;
    MouseLeavedEvent OnMouseLeaved;
    KeyUpEvent OnKeyUp;
    KeyDownEvent OnKeyDown;
    PaintEvent OnPaint;
    GridViewCheckStateChangedEvent OnGridViewCheckStateChanged;
    CloseEvent OnClose;
    MovedEvent OnMoved;
    SizeChangedEvent OnSizeChanged;
    ScrollChangedEvent OnScrollChanged;
    TextChangedEvent OnTextChanged;
    CharInputEvent OnCharInput;
    GotFocusEvent OnGotFocus;
    LostFocusEvent OnLostFocus;
    DropFileEvent OnDropFile;
    FormClosingEvent OnFormClosing;
    FormClosedEvent OnFormClosed;
    //菜单事件,id表示菜单项的id,info表示菜单项的信息
    CommandEvent OnCommand;

    HWND Handle;
    bool MinBox = true;
    bool MaxBox = true;
    bool CenterTitle = true;
    bool ControlChanged = false;
    class Control* Selected = NULL;
    class Control* UnderMouse = NULL;
    List<class Control*> Controls = List<class Control*>();
    List<class Control*> ForeGroundControls = List<class Control*>();
    Graphics* Render;
    int HeadHeight = 24;
    D2D1_COLOR_F BackColor = Colors::WhiteSmoke;
    D2D1_COLOR_F ForeColor = Colors::Black;
    ID2D1Bitmap* Image = NULL;
    ImageSizeMode SizeMode = ImageSizeMode::Normal;

    PROPERTY(POINT, Location);
    GET(POINT, Location);
    SET(POINT, Location);
    PROPERTY(SIZE, Size);
    GET(SIZE, Size);
    SET(SIZE, Size);
    READONLY_PROPERTY(SIZE, ClientSize);
    GET(SIZE, ClientSize);
    PROPERTY(std::wstring, Text);
    GET(std::wstring, Text);
    SET(std::wstring, Text);
    PROPERTY(bool, ShowInTaskBar);
    GET(bool, ShowInTaskBar);
    SET(bool, ShowInTaskBar);
    PROPERTY(bool, TopMost);
    GET(bool, TopMost);
    SET(bool, TopMost);
    PROPERTY(bool, Enable);
    GET(bool, Enable);
    SET(bool, Enable);
    PROPERTY(bool, Visable);
    GET(bool, Visable);
    SET(bool, Visable);

    HICON Icon = NULL;
    HMENU Menu = NULL;
    DWORD Style = WS_POPUP;
    Form(std::wstring _text = L"NativeWindow", POINT _location = { 0,0 }, SIZE _size = { 600,400 });
    void Show();
    void ShowDialog();
    void Close();

    template<typename T>
    T AddControl(T c)
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

    virtual bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof);
    virtual bool Update();
    virtual void RenderImage();
    D2D1_POINT_2F LastChildRB();
    D2D1_POINT_2F MaxChildRB();
    Control* LastChild();
    static bool DoEvent();
    //如果需要实现动画效果或者存在循环刷新绘制的控件请使用DoEvent,因为WaiteEvent会阻塞线程直到新消息到达
    static bool WaiteEvent();
    static LRESULT CALLBACK WINMSG_PROCESS(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};