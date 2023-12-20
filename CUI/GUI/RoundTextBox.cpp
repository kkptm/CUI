#pragma once
#include "RoundTextBox.h"
#include "TextBox.h"
#include "Form.h"
#pragma comment(lib, "Imm32.lib")
RoundTextBox::RoundTextBox(std::wstring text, int x, int y, int width, int height) :TextBox(text, x, y, width, height)
{
	this->TextMargin = this->Height * 0.5f;
}
void RoundTextBox::Update()
{
	if (!IsVisual) return;

	bool isUnderMouse = ParentForm->UnderMouse == this;
	auto d2d = Render;
	auto font = Font ? Font : d2d->DefaultFontObject;
	float render_height = Height - (TextMargin * 2.0f);
	textSize = font->GetTextSize(Text, FLT_MAX, render_height);
	float OffsetY = max((Height - textSize.height) * 0.5f, 0.0f);

	auto abslocation = AbsLocation;
	auto size = ActualSize();
	auto absRect = AbsRect;
	bool isSelected = ParentForm->Selected == this;

	d2d->PushDrawRect(absRect.left, absRect.top, absRect.right - absRect.left, absRect.bottom - absRect.top);
	{
		d2d->FillRoundRect(abslocation.x, abslocation.y, size.cx, size.cy, isSelected ? FocusedColor : BackColor, TextMargin);
		RenderImage();
		d2d->PushDrawRect(absRect.left + TextMargin, absRect.top, (absRect.right - absRect.left) - (TextMargin * 2.0f), (absRect.bottom - absRect.top));

		if (Text.size() > 0)
		{
			int sels = SelectionStart <= SelectionEnd ? SelectionStart : SelectionEnd;
			int sele = SelectionEnd >= SelectionStart ? SelectionEnd : SelectionStart;
			int selLen = sele - sels;
			auto selRange = font->HitTestTextRange(this->Text, FLT_MAX, render_height, (UINT32)sels, (UINT32)selLen);

			for (auto sr : selRange)
			{
				d2d->FillRect(sr.left + abslocation.x + TextMargin - OffsetX, (sr.top + abslocation.y) + OffsetY, sr.width, sr.height, this->SelectedBackColor);
			}

			if (isSelected)
			{
				d2d->DrawLine({ selRange[0].left + abslocation.x + TextMargin - OffsetX,(selRange[0].top + abslocation.y) - OffsetY },
					{ selRange[0].left + abslocation.x + TextMargin - OffsetX,(selRange[0].top + abslocation.y + selRange[0].height) + OffsetY }, Colors::Black);

				d2d->DrawStringLayout(Text, (float)abslocation.x + TextMargin - OffsetX, ((float)abslocation.y) + OffsetY, FLT_MAX, render_height,
					ForeColor, DWRITE_TEXT_RANGE{ (UINT32)selRange[0].top, (UINT32)selRange[0].height }, SelectedForeColor, font);
			}
			else
			{
				d2d->DrawStringLayout(Text, (float)abslocation.x + TextMargin - OffsetX, ((float)abslocation.y) + OffsetY, FLT_MAX, render_height,
					ForeColor, DWRITE_TEXT_RANGE{ 0, 0 }, SelectedForeColor, font);
			}
		}
		else if (isSelected)
		{
			d2d->DrawLine({ (float)TextMargin + (float)abslocation.x - OffsetX, (float)abslocation.y + OffsetY },
				{ (float)TextMargin + (float)abslocation.x - OffsetX, (float)abslocation.y + OffsetY + 16.0f }, Colors::Black);
		}
		d2d->PopDrawRect();
	}

	if (!Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}

	d2d->PopDrawRect();
}