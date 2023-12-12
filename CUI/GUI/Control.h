#pragma once
#include "../Graphics/Colors.h"
#include "../Graphics/Font.h"
#include "../Graphics/Graphics.h"
#include "../Utils/Event.h"
#include <string>
#include <vector>
#include <Windows.h>

enum class ImageSizeMode : char
{
	Normal,
	CenterImage,
	StretchIamge,
	Zoom
};
enum class UIClass : int
{
	UI_Base,
	UI_Label,
	UI_Button,
	UI_PictureBox,
	UI_TextBox,
	UI_RichTextBox,
	UI_PasswordBox,
	UI_ComboBox,
	UI_GridView,
	UI_CheckBox,
	UI_RadioBox,
	UI_ProgressBar,
	UI_TreeView,
	UI_Panel,
	UI_TabPage,
	UI_TabControl,
	UI_Switch,
	UI_CUSTOM
};

typedef Event<void(*)(class Control*, EventArgs)> EventHandler;
typedef Event<void(*)(class Control*)> CheckedEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseWheelEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseMoveEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseUpEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseDownEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseDoubleClickEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseClickEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseEnterEvent;
typedef Event<void(*)(class Control*, MouseEventArgs)> MouseLeavedEvent;
typedef Event<void(*)(class Control*, KeyEventArgs)> KeyUpEvent;
typedef Event<void(*)(class Control*, KeyEventArgs)> KeyDownEvent;
typedef Event<void(*)(class Control*)> PaintEvent;
typedef Event<void(*)(class Control*, int column, int row, bool value)> GridViewCheckStateChangedEvent;
typedef Event<void(*)(class Control*)> CloseEvent;
typedef Event<void(*)(class Control*)> MovedEvent;
typedef Event<void(*)(class Control*)> SizeChangedEvent;
typedef Event<void(*)(class Control*)> ScrollChangedEvent;
typedef Event<void(*)(class Control*, std::wstring, std::wstring)> TextChangedEvent;
typedef Event<void(*)(class Control*, wchar_t)> CharInputEvent;
typedef Event<void(*)(class Control*)> GotFocusEvent;
typedef Event<void(*)(class Control*)> LostFocusEvent;
typedef Event<void(*)(class Control*, List<std::wstring>)> DropFileEvent;
typedef Event<void(*)(class Control*)> SelectionChangedEvent;

#define defthis(x) decltype(this) _this = (decltype(this))((class Control*)x)->ParentForm
class Control
{
private:
	POINT _location = { 0,0 };
	SIZE _size = { 120,20 };
	D2D1_COLOR_F _backcolor = Colors::gray91;
	D2D1_COLOR_F _forecolor = Colors::Black;
	D2D1_COLOR_F _boldercolor = Colors::Black;
	class ID2D1Bitmap* _image = NULL;
	std::wstring _text;
	List<Control*> Children;
public:
	CheckedEvent OnChecked;
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
	class Form* ParentForm;
	class Control* Parent;
	bool TextChanged = true;
	bool Enable;
	bool Visable;
	bool Checked;
	UINT64 Tag;
	ImageSizeMode SizeMode = ImageSizeMode::Zoom;
	Font* Font;
	Graphics* Render;
	Control();
	~Control();
	virtual UIClass Type();
	virtual void Update();
	virtual void PostRender();
	READONLY_PROPERTY(int, Count);
	GET(int, Count);
	Control* operator[](int index);
	Control* get(int index);

	template<typename T>
	T AddControl(T c) {
		if (c->Parent) {
			throw std::exception("�ÿؼ���������������!");
			return NULL;
		}
		if (this->Children.Contains(c)) {
			return c;
		}
		c->Parent = this;
		c->ParentForm = this->ParentForm;
		c->Render = this->Render;
		this->Children.Add(c);
		return c;
	}
	void RemoveControl(Control* c);
	READONLY_PROPERTY(POINT, AbsLocation);
	GET(POINT, AbsLocation);
	READONLY_PROPERTY(D2D1_RECT_F, AbsRect);
	GET(D2D1_RECT_F, AbsRect);
	READONLY_PROPERTY(bool, IsVisual);
	GET(bool, IsVisual);
	PROPERTY(POINT, Location);
	GET(POINT, Location);
	SET(POINT, Location);
	PROPERTY(SIZE, Size);
	GET(SIZE, Size);
	SET(SIZE, Size);
	PROPERTY(int, Left);
	GET(int, Left);
	SET(int, Left);
	PROPERTY(int, Top);
	GET(int, Top);
	SET(int, Top);
	PROPERTY(int, Width);
	GET(int, Width);
	SET(int, Width);
	PROPERTY(int, Height);
	GET(int, Height);
	SET(int, Height);
	READONLY_PROPERTY(float, Right);
	GET(float, Right);
	READONLY_PROPERTY(float, Bottom);
	GET(float, Bottom);
	PROPERTY(std::wstring, Text);
	GET(std::wstring, Text);
	SET(std::wstring, Text);
	PROPERTY(D2D1_COLOR_F, BolderColor);
	GET(D2D1_COLOR_F, BolderColor);
	SET(D2D1_COLOR_F, BolderColor);
	PROPERTY(D2D1_COLOR_F, BackColor);
	GET(D2D1_COLOR_F, BackColor);
	SET(D2D1_COLOR_F, BackColor);
	PROPERTY(D2D1_COLOR_F, ForeColor);
	GET(D2D1_COLOR_F, ForeColor);
	SET(D2D1_COLOR_F, ForeColor);
	PROPERTY(class ID2D1Bitmap*, Image);
	GET(class ID2D1Bitmap*, Image);
	SET(class ID2D1Bitmap*, Image);
	virtual void RenderImage();
	virtual SIZE ActualSize();
	void setTextPrivate(std::wstring);
	virtual bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof);
};