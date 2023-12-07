#pragma once
#include "Switch.h"
#include "Form.h"
#pragma comment(lib, "Imm32.lib")
UIClass Switch::Type() { return UIClass::UI_Switch; }
Switch::Switch(int x, int y, int width, int height)
{
	this->Location = POINT{ x,y };
	this->Size = SIZE{ width,height };
	auto bc = this->BackColor;
	bc.a = 0.0f;
	this->BackColor = bc;
}
void Switch::Update()
{
	if (this->IsVisual == false)return;
	bool isUnderMouse = this->ParentForm->UnderMouse == this;
	bool isSelected = this->ParentForm->Selected == this;
	auto d2d = this->Render;
	auto abslocation = this->AbsLocation;
	auto size = this->ActualSize();
	auto absRect = this->AbsRect;
	if (last_width > size.cx)
	{
		absRect.right += last_width - size.cx;
		size.cx = last_width;
	}
	d2d->PushDrawRect(absRect.left, absRect.top, absRect.right - absRect.left, absRect.bottom - absRect.top);
	{
		float r = size.cy / 2.0f;
		float x1 = abslocation.x + r;
		float x2 = abslocation.x + (size.cx - r);
		float y = abslocation.y + r;
		d2d->FillEllipse({ {x1,y},r,r }, this->Checked ? Colors::Green : Colors::Red);
		d2d->FillEllipse({ {x2,y},r,r }, this->Checked ? Colors::Green : Colors::Red);
		d2d->FillRect(x1, abslocation.y, x2 - x1, size.cy, this->Checked ? Colors::Green : Colors::Red);
		d2d->FillEllipse({ {this->Checked ? x2 : x1,y},r - 2.0f,r - 2.0f }, Colors::White);
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
	last_width = size.cx;
}
bool Switch::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
{
	if (!this->Enable || !this->Visable) return true;
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
	case WM_MOUSEWHEEL://mouse wheel
	{
		MouseEventArgs event_obj = MouseEventArgs(MouseButtons::None, 0, xof, yof, GET_WHEEL_DELTA_WPARAM(wParam));
		this->OnMouseWheel(this, event_obj);
	}
	break;
	case WM_MOUSEMOVE://mouse move
	{
		this->ParentForm->UnderMouse = this;
		MouseEventArgs event_obj = MouseEventArgs(MouseButtons::None, 0, xof, yof, HIWORD(wParam));
		this->OnMouseMove(this, event_obj);
	}
	break;
	case WM_LBUTTONDOWN://mouse down
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		if (WM_LBUTTONDOWN == message)
		{
			auto lastSelected = this->ParentForm->Selected;
			this->ParentForm->Selected = this;
			if (lastSelected && lastSelected != this)
			{
				lastSelected->PostRender();
			}
		}
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseDown(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_LBUTTONUP://mouse up
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		if (WM_LBUTTONUP == message && this->ParentForm->Selected == this)
		{
			this->Checked = !this->Checked;
			MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
			this->OnMouseClick(this, event_obj);
		}
		this->ParentForm->Selected = NULL;
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseUp(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_LBUTTONDBLCLK://mouse double click
	{
		auto lastSelected = this->ParentForm->Selected;
		this->ParentForm->Selected = this;
		if (lastSelected && lastSelected == this)
		{
			this->Checked = !this->Checked;
			MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
			this->OnMouseDoubleClick(this, event_obj);
			this->PostRender();
		}
	}
	break;
	case WM_KEYDOWN://keyboard down
	{
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyDown(this, event_obj);
	}
	break;
	case WM_KEYUP://keyboard up
	{
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyUp(this, event_obj);
	}
	break;
	}
	return true;
}