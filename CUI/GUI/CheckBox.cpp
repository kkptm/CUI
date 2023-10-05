#pragma once
#include "CheckBox.h"
#include "Form.h"
UIClass CheckBox::Type() { return UIClass::UI_CheckBox; }
D2D1_COLOR_F UnderMouseColor = Colors::DarkSlateGray;
CheckBox::CheckBox(std::wstring text, int x, int y)
{
	this->Text = text;
	this->Location = POINT{ x,y };
	this->BackColor = D2D1_COLOR_F{ 0.75f , 0.75f , 0.75f , 0.75f };
}
SIZE CheckBox::ActualSize()
{
	auto d2d = this->Render;
	auto font = this->Font ? this->Font : d2d->DefaultFontObject;
	auto text_size = font->GetTextSize(this->Text);
	return SIZE{ (int)text_size.width + int(text_size.height + 2),(int)text_size.height };
}
void CheckBox::Update()
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
		auto col = this->ForeColor;
		if (isUnderMouse)
		{
			col = UnderMouseColor;
		}
		auto font = this->Font ? this->Font : d2d->DefaultFontObject;
		auto textSize = font->GetTextSize(this->Text);
		d2d->DrawString(this->Text, abslocation.x + textSize.height + 2, abslocation.y, col);
		d2d->DrawRect(
			abslocation.x + (textSize.height * 0.2), abslocation.y + (textSize.height * 0.2),
			textSize.height * 0.6, textSize.height * 0.6,
			col);
		if (this->Checked)
		{
			d2d->FillRect(
				abslocation.x + (textSize.height * 0.35), abslocation.y + (textSize.height * 0.35),
				textSize.height * 0.3, textSize.height * 0.3,
				col);
		}
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
	last_width = size.cx;
}
bool CheckBox::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
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
				lastSelected->SingleUpdate();
			}
		}
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseDown(this, event_obj);
		this->SingleUpdate();
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
		this->SingleUpdate();
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
			this->SingleUpdate();
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

