#include "CustomControls.h"
#include "GUI/Form.h"

CustomTextBox1::CustomTextBox1(std::wstring text, int x, int y, int width, int height) :TextBox(text, x, y, width, height)
{
	this->TextMargin = this->Height * 0.5f;
}
void CustomTextBox1::Update()
{
	if (this->IsVisual == false)return;
	bool isUnderMouse = this->ParentForm->UnderMouse == this;
	auto d2d = this->Render;
	auto font = this->Font ? this->Font : d2d->DefaultFontObject;
	float render_height = this->Height - (TextMargin * 2.0f);
	textSize = font->GetTextSize(this->Text, FLT_MAX, render_height);
	float OffsetY = (this->Height - textSize.height) * 0.5f;
	if (OffsetY < 0.0f)OffsetY = 0.0f;
	auto abslocation = this->AbsLocation;
	auto size = this->ActualSize();
	auto absRect = this->AbsRect;
	bool isSelected = this->ParentForm->Selected == this;
	d2d->PushDrawRect(absRect.left, absRect.top, absRect.right - absRect.left, absRect.bottom - absRect.top);
	{
		d2d->FillRoundRect(abslocation.x, abslocation.y, size.cx, size.cy, isSelected ? this->FocusedColor : this->BackColor, this->TextMargin);
		if (this->Image)
		{
			this->RenderImage();
		}
		D2D1_GRADIENT_STOP stops[] =
		{
			{ 0.0f, D2D1::ColorF(227.0f / 255.0f, 9.0f / 255.0f, 64.0f / 255.0f, 1.0f) },
			{ 0.33f, D2D1::ColorF(231.0f / 255.0f, 215.0f / 255.0f, 2.0f / 255.0f, 1.0f) },
			{ 0.66f, D2D1::ColorF(15.0f / 255.0f, 168.0f / 255.0f, 149.0f / 255.0f, 1.0f) },
			{ 1.0f, D2D1::ColorF(19.0f / 255.0f, 115.0f / 255.0f, 232.0f / 255.0f, 1.0f) }
		};
		d2d->PushDrawRect(absRect.left + this->TextMargin, absRect.top, (absRect.right - absRect.left) - (this->TextMargin * 2.0f), (absRect.bottom - absRect.top));
		auto brush = d2d->CreateLinearGradientBrush(stops, _countof(stops));
		brush->SetStartPoint(D2D1::Point2F(this->Left, this->Top));
		brush->SetEndPoint(D2D1::Point2F(this->Left + this->Width, this->Top + this->Height));
		if (this->Text.size() > 0)
		{
			auto font = this->Font ? this->Font : d2d->DefaultFontObject;
			if (isSelected)
			{
				int sels = SelectionStart <= SelectionEnd ? SelectionStart : SelectionEnd;
				int sele = SelectionEnd >= SelectionStart ? SelectionEnd : SelectionStart;
				int selLen = sele - sels;
				auto selRange = font->HitTestTextRange(this->Text, FLT_MAX, render_height, (UINT32)sels, (UINT32)selLen);
				if (selLen != 0)
				{
					for (auto sr : selRange)
					{
						d2d->FillRect(sr.left + abslocation.x + TextMargin - OffsetX, (sr.top + abslocation.y) + OffsetY, sr.width, sr.height, this->SelectedBackColor);
					}
				}
				else
				{
					d2d->DrawLine(
						{ selRange[0].left + abslocation.x + TextMargin - OffsetX,(selRange[0].top + abslocation.y) - OffsetY },
						{ selRange[0].left + abslocation.x + TextMargin - OffsetX,(selRange[0].top + abslocation.y + selRange[0].height) + OffsetY },
						Colors::Black);
				}
				d2d->DrawStringLayout(this->Text,
					(float)abslocation.x + TextMargin - OffsetX, ((float)abslocation.y) + OffsetY,
					FLT_MAX, render_height,
					brush,
					DWRITE_TEXT_RANGE{ (UINT32)sels, (UINT32)selLen },
					this->SelectedForeColor,
					font);
			}
			else
			{
				d2d->DrawStringLayout(this->Text,
					(float)abslocation.x + TextMargin - OffsetX, ((float)abslocation.y) + OffsetY,
					FLT_MAX, render_height,
					brush,
					font);
			}
		}
		else
		{
			if (isSelected)
				d2d->DrawLine(
					{ (float)TextMargin + (float)abslocation.x - OffsetX, (float)abslocation.y + OffsetY },
					{ (float)TextMargin + (float)abslocation.x - OffsetX, (float)abslocation.y + OffsetY + 16.0f },
					Colors::Red);
		}
		//d2d->DrawRoundRect(
		//	abslocation.x + (this->Boder * 0.5f), abslocation.y + (this->Boder * 0.5f),
		//	size.cx - this->Boder, size.cy - this->Boder,
		//	Colors::Red,
		//	this->Boder,
		//	this->TextMargin);
		d2d->DrawLine(
			{ abslocation.x + this->TextMargin, abslocation.y + this->textSize.height + OffsetY },
			{ abslocation.x + (this->Width - this->TextMargin), abslocation.y + this->textSize.height + OffsetY },
			brush,
			1.0f
		);
		//d2d->DrawLine(
		//	{ abslocation.x + this->TextMargin, abslocation.y + OffsetY },
		//	{ abslocation.x + (this->Width - this->TextMargin), abslocation.y + OffsetY },
		//	brush,
		//	1.0f
		//);
		brush->Release();
		d2d->PopDrawRect();
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
}
CustomLabel1::CustomLabel1(std::wstring text, int x, int y) :Label(text, x, y)
{
}
void CustomLabel1::Update()
{
	if (this->IsVisual == false)return;
	auto d2d = this->Render;
	auto abslocation = this->AbsLocation;
	auto size = this->ActualSize();
	auto absRect = this->AbsRect;
	if (last_width > size.cx)
	{
		absRect.right += last_width - size.cx;
		size.cx = last_width;
	}
	d2d->PushDrawRect(absRect.left, absRect.top, FLT_MAX, FLT_MAX);
	{
		D2D1_GRADIENT_STOP stops[] =
		{
			{ 0.0f, D2D1::ColorF(227.0f / 255.0f, 9.0f / 255.0f, 64.0f / 255.0f, 1.0f) },
			{ 0.33f, D2D1::ColorF(231.0f / 255.0f, 215.0f / 255.0f, 2.0f / 255.0f, 1.0f) },
			{ 0.66f, D2D1::ColorF(15.0f / 255.0f, 168.0f / 255.0f, 149.0f / 255.0f, 1.0f) },
			{ 1.0f, D2D1::ColorF(19.0f / 255.0f, 115.0f / 255.0f, 232.0f / 255.0f, 1.0f) }
		};
		auto brush = d2d->CreateLinearGradientBrush(stops, _countof(stops));
		brush->SetStartPoint(D2D1::Point2F(this->Left, this->Top));
		brush->SetEndPoint(D2D1::Point2F(this->Left + this->Width, this->Top + this->Height));
		if (this->Image)
		{
			this->RenderImage();
		}
		d2d->DrawString(this->Text, abslocation.x, abslocation.y, brush, this->Font);
		brush->Release();
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
	last_width = size.cx;
}
