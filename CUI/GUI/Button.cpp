#include "Button.h"
#include "Form.h"
UIClass Button::Type() { return UIClass::UI_Button; }
Button::Button(std::wstring text, int x, int y, int width, int height)
{
	this->Text = text;
	this->Location = POINT{ x,y };
	this->Size = SIZE{ width,height };
	this->BackColor = Colors::Snow3;
	this->BolderColor = Colors::Snow4;
}
void Button::Update()
{
	if (this->IsVisual == false)return;
	bool isUnderMouse = this->ParentForm->UnderMouse == this;
	bool isSelected = this->ParentForm->Selected == this;
	auto d2d = this->Render;
	auto abslocation = this->AbsLocation;
	auto size = this->ActualSize();
	auto absRect = this->AbsRect;
	d2d->PushDrawRect(absRect.left, absRect.top, absRect.right - absRect.left, absRect.bottom - absRect.top);
	{
		d2d->FillRoundRect(abslocation.x + (this->Boder * 0.5f), abslocation.y + (this->Boder * 0.5f), size.cx - this->Boder, size.cy - this->Boder, this->BackColor, this->Image ? 0.0f : this->Height * Round);
		if (this->Image)
		{
			this->RenderImage();
		}
		if (isUnderMouse && isSelected)
		{
			d2d->FillRoundRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.7f }, this->Image ? 0.0f : this->Height * Round);
		}
		else if (isUnderMouse)
		{
			d2d->FillRoundRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.4f }, this->Image ? 0.0f : this->Height * Round);
		}
		auto font = this->Font ? this->Font : d2d->DefaultFontObject;
		auto textSize = font->GetTextSize(this->Text);
		float drawLeft = 0.0f;
		float drawTop = 0.0f;
		if (this->Width > textSize.width)
		{
			drawLeft = (this->Width - textSize.width) / 2.0f;
		}
		if (this->Height > textSize.height)
		{
			drawTop = (this->Height - textSize.height) / 2.0f;
		}
		d2d->DrawString(this->Text, abslocation.x + drawLeft, abslocation.y + drawTop, this->ForeColor, this->Font);
		d2d->DrawRoundRect(abslocation.x + (this->Boder * 0.5f), abslocation.y + (this->Boder * 0.5f),
			size.cx - this->Boder, size.cy - this->Boder,
			this->BolderColor, this->Boder, this->Image ? 0.0f : this->Height * this->Round);
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
}
bool Button::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
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
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseDoubleClick(this, event_obj);
		this->PostRender();
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