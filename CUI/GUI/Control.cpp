#include "Control.h"
#include "Form.h"

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)

#ifndef PROPERTY_CPP
#define GET_CPP(c,t,n) t c::property__get_##n() 
#define SET_CPP(c,t,n) void c::property__set_##n(t value)
#endif // !PROPERTY_CPP
Control::Control()
	:
	Enable(true),
	Visable(true),
	Checked(false),
	ParentForm(nullptr),
	Parent(nullptr),
	Font(nullptr),
	Tag(NULL),
	SizeMode(ImageSizeMode::Zoom),
	_image(NULL),
	_text(L"")
{
}
Control::~Control()
{
	if (this->Font)
	{
		delete this->Font;
	}
	for (auto c : this->Children)
	{
		delete c;
	}
}
UIClass Control::Type() { return UIClass::UI_Base; }

void Control::Update() {}
void Control::SingleUpdate()
{
	if(this->ParentForm) this->ParentForm->ControlChanged = true;
}
GET_CPP(Control, int, Count)
{
	return this->Children.Count;
}
Control* Control::operator[](int index)
{
	return this->Children[index];
}
Control* Control::get(int index)
{
	return this->Children[index];
}
Control* Control::AddControl(Control* c)
{
	if (c->Parent)
	{
		throw "该控件已属于其他容器!";
		return NULL;
	}
	if (this->Children.Contains(c))
	{
		return c;
	}
	c->Parent = this;
	c->ParentForm = this->ParentForm;
	c->Render = this->Render;
	this->Children.Add(c);
	return c;
}
void Control::RemoveControl(Control* c)
{
	this->Children.Remove(c);
	c->Parent = NULL;
	c->ParentForm = NULL;
	c->Render = NULL;
}
GET_CPP(Control, POINT, AbsLocation)
{
	Control* tmpc = this;
	POINT tmpl = tmpc->_location;
	while (tmpc->Parent)
	{
		tmpc = tmpc->Parent;
		auto loc = tmpc->_location;
		tmpl.x += loc.x;
		tmpl.y += loc.y;
	}
	return tmpl;
}
GET_CPP(Control, D2D1_RECT_F, AbsRect)
{
	Control* tmpc = this;
	auto absMin = this->AbsLocation;
	return D2D1_RECT_F{ (float)absMin.x,(float)absMin.y,(float)absMin.x + this->_size.cx,(float)absMin.y + this->_size.cy };
}
GET_CPP(Control, bool, IsVisual)
{
	if (this->Visable == false) return false;
	Control* tmpc = this;
	while (tmpc->Parent)
	{
		tmpc = tmpc->Parent;
		if (tmpc->Visable == false) return false;
	}
	return true;
}
GET_CPP(Control, POINT, Location)
{
	return _location;
}
SET_CPP(Control, POINT, Location)
{
	this->OnMoved(this);
	_location = value;
}
GET_CPP(Control, SIZE, Size)
{
	return _size;
}
SET_CPP(Control, SIZE, Size)
{
	this->OnSizeChanged(this);
	_size = value;
}
GET_CPP(Control, int, Left)
{
	return this->Location.x;
}
SET_CPP(Control, int, Left)
{
	this->_location = POINT{ value,this->_location.y };
}
GET_CPP(Control, int, Top)
{
	return this->Location.y;
}
SET_CPP(Control, int, Top)
{
	this->_location = POINT{ this->_location.x,value };
}
GET_CPP(Control, int, Width)
{
	return this->_size.cx;
}
SET_CPP(Control, int, Width)
{
	this->OnSizeChanged(this);
	this->_size.cx = value;
}
GET_CPP(Control, int, Height)
{
	return this->_size.cy;
}
SET_CPP(Control, int, Height)
{
	this->OnSizeChanged(this);
	_size.cy = value;
}
GET_CPP(Control, float, Right)
{
	return this->Left + this->Width;
}
GET_CPP(Control, float, Bottom)
{
	return this->Top + this->Height;
}
GET_CPP(Control, std::wstring, Text)
{
	return _text;
}
SET_CPP(Control, std::wstring, Text)
{
	if (value != _text)
		this->OnTextChanged(this, _text, value);
	_text = value;
}
GET_CPP(Control, D2D1_COLOR_F, BolderColor)
{
	return _boldercolor;
}
SET_CPP(Control, D2D1_COLOR_F, BolderColor)
{
	_boldercolor = value;
}
GET_CPP(Control, D2D1_COLOR_F, BackColor)
{
	return _backcolor;
}
SET_CPP(Control, D2D1_COLOR_F, BackColor)
{
	_backcolor = value;
}
GET_CPP(Control, D2D1_COLOR_F, ForeColor)
{
	return _forecolor;
}
SET_CPP(Control, D2D1_COLOR_F, ForeColor)
{
	_forecolor = value;
}
GET_CPP(Control, ID2D1Bitmap*, Image)
{
	return _image;
}
SET_CPP(Control, ID2D1Bitmap*, Image)
{
	_image = value;
}
void Control::RenderImage()
{
	if (this->_image)
	{
		auto absLocation = this->AbsLocation;
		auto size = this->_image->GetSize();
		if (size.width > 0 && size.height > 0)
		{
			auto asize = this->ActualSize();
			switch (this->SizeMode)
			{
			case ImageSizeMode::Normal:
			{
				this->Render->DrawBitmap(this->_image, absLocation.x, absLocation.y, size.width, size.height);
			}
			break;
			case ImageSizeMode::CenterImage:
			{
				float xf = (asize.cx - size.width) / 2.0f;
				float yf = (asize.cy - size.height) / 2.0f;
				this->Render->DrawBitmap(this->_image, absLocation.x + xf, absLocation.y + yf, size.width, size.height);
			}
			break;
			case ImageSizeMode::StretchIamge:
			{
				this->Render->DrawBitmap(this->_image, absLocation.x, absLocation.y, asize.cx, asize.cy);
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
				this->Render->DrawBitmap(this->_image, absLocation.x + xf, absLocation.y + yf, tw, th);
			}
			break;
			default:
				break;
			}
		}
	}
}
SIZE Control::ActualSize()
{
	return this->Size;
}
bool Control::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
{
	if (!this->Enable || !this->Visable) return true;
	if (WM_LBUTTONDOWN == message)
	{
		if (this->ParentForm->Selected && this->ParentForm->Selected != this)
		{
			auto se = this->ParentForm->Selected;
			this->ParentForm->Selected = this;
			se->SingleUpdate();
		}
	}
	switch (message)
	{
	case WM_DROPFILES:
	{
		HDROP hDropInfo = HDROP(wParam);
		UINT uFileNum = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
		TCHAR strFileName[MAX_PATH];
		List<std::wstring> files;
		for (int i = 0; i < uFileNum; i++)
		{
			DragQueryFile(hDropInfo, i, strFileName, MAX_PATH);
			files.Add(strFileName);
		}
		DragFinish(hDropInfo);
		if (files.Count > 0)
		{
			this->OnDropFile(this, files);
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		MouseEventArgs event_obj = MouseEventArgs(MouseButtons::None, 0, xof, yof, GET_WHEEL_DELTA_WPARAM(wParam));
		this->OnMouseWheel(this, event_obj);
	}
	break;
	case WM_MOUSEMOVE:
	{
		MouseEventArgs event_obj = MouseEventArgs(MouseButtons::None, 0, xof, yof, HIWORD(wParam));
		this->OnMouseMove(this, event_obj);
	}
	break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		if (WM_LBUTTONDOWN == message)
		{
			this->ParentForm->Selected = this;
		}
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseDown(this, event_obj);
		this->SingleUpdate();
	}
	break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseUp(this, event_obj);
		this->SingleUpdate();
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseDoubleClick(this, event_obj);
	}
	break;
	case WM_KEYDOWN:
	{
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyDown(this, event_obj);
	}
	break;
	case WM_KEYUP:
	{
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyUp(this, event_obj);
	}
	break;
	}
	return true;
}