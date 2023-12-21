#pragma once
#include "Label.h"
#include "Form.h"
#pragma comment(lib, "Imm32.lib")
UIClass Label::Type() { return UIClass::UI_Label; }
Label::Label(std::wstring text, int x, int y)
{
	this->Text = text;
	this->Location = POINT{ x,y };
	this->BackColor = D2D1_COLOR_F{ .0f,.0f,.0f,.0f };
}
SIZE Label::ActualSize()
{
	auto d2d = this->Render;
	auto font = this->Font;
	auto text_size = font->GetTextSize(this->Text);
	return SIZE{ (int)text_size.width,(int)text_size.height };
}
void Label::Update()
{
	if (this->IsVisual == false)return;
	auto d2d = this->Render;
	auto abslocation = this->AbsLocation;
	auto size = this->ActualSize();
	auto absRect = this->AbsRect;
	auto font = this->Font;
	if (last_width > size.cx)
	{
		absRect.right += last_width - size.cx;
		size.cx = last_width;
	}
	d2d->PushDrawRect(absRect.left, absRect.top, FLT_MAX, FLT_MAX);
	{
		if (this->Image)
		{
			this->RenderImage();
		}
		d2d->DrawString(this->Text, abslocation.x, abslocation.y, this->ForeColor, font);
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
	last_width = size.cx;
}