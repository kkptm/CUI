#pragma once
#include "PasswordBox.h"
#include "Form.h"
#pragma comment(lib, "Imm32.lib")
UIClass PasswordBox::Type() { return UIClass::UI_TextBox; }
PasswordBox::PasswordBox(std::wstring text, int x, int y, int width, int height)
{
	this->Text = text;
	this->Location = POINT{ x,y };
	this->Size = SIZE{ width,height };
	this->BackColor = D2D1_COLOR_F{ 0.75f , 0.75f , 0.75f , 0.75f };
}
void PasswordBox::InputText(std::wstring input)
{
	int sels = SelectionStart <= SelectionEnd ? SelectionStart : SelectionEnd;
	int sele = SelectionEnd >= SelectionStart ? SelectionEnd : SelectionStart;
	if (sele >= this->Text.size() && sels >= this->Text.size())
	{
		this->Text += input;
		SelectionEnd = SelectionStart = this->Text.size();
	}
	else
	{
		List<wchar_t> tmp = List<wchar_t>();
		tmp.AddRange((wchar_t*)this->Text.c_str(), this->Text.size());
		if (sele > sels)
		{
			int sublen = sele - sels;
			for (int i = 0; i < sublen; i++)
			{
				tmp.RemoveAt(sels);
			}
			for (int i = 0; i < input.size(); i++)
			{
				tmp.Insert(sels + i, input[i]);
			}
			SelectionEnd = SelectionStart = sels + (input.size());
			tmp.Add(L'\0');
			this->Text = std::wstring(tmp.Pointer());
		}
		else if (sele == sels && sele >= 0)
		{
			for (int i = 0; i < input.size(); i++)
			{
				tmp.Insert(sels + i, input[i]);
			}
			SelectionEnd += input.size();
			SelectionStart += input.size();
			tmp.Add(L'\0');
			this->Text = std::wstring(tmp.Pointer());
		}
		else
		{
			this->Text += input;
			SelectionEnd = SelectionStart = this->Text.size();
		}
	}
	List<wchar_t> tmp = List<wchar_t>();
	tmp.AddRange((wchar_t*)this->Text.c_str(), this->Text.size() + 1);
	for (int i = 0; i < tmp.Count; i++)
	{
		if (tmp[i] == L'\r' || tmp[i] == L'\n')
		{
			tmp[i] = L' ';
		}
	}
	this->Text = std::wstring(tmp.Pointer());
}
void PasswordBox::InputBack()
{
	int sels = SelectionStart <= SelectionEnd ? SelectionStart : SelectionEnd;
	int sele = SelectionEnd >= SelectionStart ? SelectionEnd : SelectionStart;
	int selLen = sele - sels;
	if (selLen > 0)
	{
		List<wchar_t> tmp = List<wchar_t>((wchar_t*)this->Text.c_str(), this->Text.size());
		for (int i = 0; i < selLen; i++)
		{
			tmp.RemoveAt(sels);
		}
		tmp.Add(L'\0');
		this->SelectionStart = this->SelectionEnd = sels;
		this->Text = tmp.data();
	}
	else
	{
		if (sels > 0)
		{
			List<wchar_t> tmp = List<wchar_t>((wchar_t*)this->Text.c_str(), this->Text.size());
			tmp.RemoveAt(sels - 1);
			tmp.Add(L'\0');
			this->SelectionStart = this->SelectionEnd = sels - 1;
			this->Text = tmp.data();
		}
	}
}
void PasswordBox::InputDelete()
{
	int sels = SelectionStart <= SelectionEnd ? SelectionStart : SelectionEnd;
	int sele = SelectionEnd >= SelectionStart ? SelectionEnd : SelectionStart;
	int selLen = sele - sels;
	if (selLen > 0)
	{
		List<wchar_t> tmp = List<wchar_t>((wchar_t*)this->Text.c_str(), this->Text.size());
		for (int i = 0; i < selLen; i++)
		{
			tmp.RemoveAt(sels);
		}
		tmp.Add(L'\0');
		this->SelectionStart = this->SelectionEnd = sels;
		this->Text = tmp.data();
	}
	else
	{
		if (sels < this->Text.size())
		{
			List<wchar_t> tmp = List<wchar_t>((wchar_t*)this->Text.c_str(), this->Text.size());
			tmp.RemoveAt(sels);
			tmp.Add(L'\0');
			this->SelectionStart = this->SelectionEnd = sels;
			this->Text = tmp.data();
		}
	}
}
void PasswordBox::UpdateScroll(bool arrival)
{
	float render_width = this->Width - (TextMargin * 2.0f);
	auto font = this->Font;
	std::wstring MaskText(this->Text.size(), L'*');
	auto lastSelect = font->HitTestTextRange(MaskText, FLT_MAX, FLT_MAX, (UINT32)SelectionEnd, (UINT32)0)[0];
	if ((lastSelect.left + lastSelect.width) - OffsetX > render_width)
	{
		OffsetX = (lastSelect.left + lastSelect.width) - render_width;
	}
	if (lastSelect.left - OffsetX < 0.0f)
	{
		OffsetX = lastSelect.left;
	}
}
std::wstring PasswordBox::GetSelectedString()
{
	int sels = SelectionStart <= SelectionEnd ? SelectionStart : SelectionEnd;
	int sele = SelectionEnd >= SelectionStart ? SelectionEnd : SelectionStart;
	if (sele > sels)
	{
		std::wstring s = L"";
		for (int i = sels; i < sele; i++)
		{
			s += this->Text[i];
		}
		return s;
	}
	return L"";
}
void PasswordBox::Update()
{
	if (this->IsVisual == false)return;
	bool isUnderMouse = this->ParentForm->UnderMouse == this;
	auto d2d = this->Render;
	auto font = this->Font;
	float render_height = this->Height - (TextMargin * 2.0f);
	std::wstring MaskText(this->Text.size(), L'*');
	textSize = font->GetTextSize(MaskText, FLT_MAX, render_height);
	float OffsetY = (this->Height - textSize.height) * 0.5f;
	if (OffsetY < 0.0f)OffsetY = 0.0f;
	auto abslocation = this->AbsLocation;
	auto size = this->ActualSize();
	auto absRect = this->AbsRect;
	bool isSelected = this->ParentForm->Selected == this;
	d2d->PushDrawRect(absRect.left, absRect.top, absRect.right - absRect.left, absRect.bottom - absRect.top);
	{
		std::wstring MaskText(this->Text.size(), L'*');
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, isSelected ? this->FocusedColor : this->BackColor);
		if (this->Image)
		{
			this->RenderImage();
		}
		if (this->Text.size() > 0)
		{
			auto font = this->Font;
			if (isSelected)
			{
				int sels = SelectionStart <= SelectionEnd ? SelectionStart : SelectionEnd;
				int sele = SelectionEnd >= SelectionStart ? SelectionEnd : SelectionStart;
				int selLen = sele - sels;
				auto selRange = font->HitTestTextRange(MaskText, FLT_MAX, render_height, (UINT32)sels, (UINT32)selLen);
				if (selLen != 0)
				{
					for (auto sr : selRange)
					{
						d2d->FillRect(sr.left + abslocation.x + TextMargin - OffsetX, (sr.top + abslocation.y) - OffsetY, sr.width, sr.height, this->SelectedBackColor);
					}
				}
				else
				{
					//if ((GetTickCount64() / 1000) % 2 == 0)
						d2d->DrawLine(
							{ selRange[0].left + abslocation.x + TextMargin - OffsetX,(selRange[0].top + abslocation.y) - OffsetY },
							{ selRange[0].left + abslocation.x + TextMargin - OffsetX,(selRange[0].top + abslocation.y + selRange[0].height) + OffsetY },
							Colors::Black);
				}
				d2d->DrawStringLayout(MaskText,
					(float)abslocation.x + TextMargin - OffsetX, ((float)abslocation.y) + OffsetY,
					FLT_MAX, render_height,
					this->ForeColor,
					DWRITE_TEXT_RANGE{ (UINT32)sels, (UINT32)selLen },
					this->SelectedForeColor,
					font);
			}
			else
			{
				d2d->DrawStringLayout(MaskText,
					(float)abslocation.x + TextMargin - OffsetX, ((float)abslocation.y) + OffsetY,
					FLT_MAX, render_height,
					this->ForeColor,
					DWRITE_TEXT_RANGE{ (UINT32)0, (UINT32)0 },
					this->SelectedForeColor,
					font);
			}
		}
		else
		{
			if (isSelected)
				d2d->DrawLine(
					{ (float)TextMargin + (float)abslocation.x - OffsetX, (float)abslocation.y + OffsetY },
					{ (float)TextMargin + (float)abslocation.x - OffsetX, (float)abslocation.y + OffsetY + 16.0f },
					Colors::Black);
		}
		d2d->DrawRect(abslocation.x, abslocation.y, size.cx, size.cy, this->BolderColor, this->Boder);
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
}
bool PasswordBox::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
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
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && this->ParentForm->Selected == this)
		{
			auto font = this->Font;
			float render_height = this->Height - (TextMargin * 2.0f);
			std::wstring MaskText(this->Text.size(), L'*');
			SelectionEnd = font->HitTestTextPosition(MaskText, FLT_MAX, render_height, (xof - TextMargin) + this->OffsetX, yof - TextMargin);
			UpdateScroll();
			this->PostRender();
		}
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
			if (this->ParentForm->Selected != this)
			{
				auto lse = this->ParentForm->Selected;
				this->ParentForm->Selected = this;
				if (lse) lse->PostRender();
			}
			auto font = this->Font;
			float render_height = this->Height - (TextMargin * 2.0f);
			std::wstring MaskText(this->Text.size(), L'*');
			this->SelectionStart = this->SelectionEnd = font->HitTestTextPosition(MaskText, FLT_MAX, render_height, (xof - TextMargin) + this->OffsetX, yof - TextMargin);
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
		if (this->ParentForm->Selected == this)
		{
			float render_height = this->Height - (TextMargin * 2.0f);
			auto font = this->Font;
			std::wstring MaskText(this->Text.size(), L'*');
			SelectionEnd = font->HitTestTextPosition(MaskText, FLT_MAX, render_height, (xof - TextMargin) + this->OffsetX, yof - TextMargin);
		}
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseUp(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_LBUTTONDBLCLK://mouse double click
	{
		this->ParentForm->Selected = this;
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseDoubleClick(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_KEYDOWN://keyboard down
	{
		auto pos = this->AbsLocation;
		HIMC hImc = ImmGetContext(this->ParentForm->Handle);
		COMPOSITIONFORM form;
		form.dwStyle = CFS_RECT;
		form.ptCurrentPos = pos;
		form.rcArea = RECT{ pos.x, pos.y + this->Height, pos.x + 300, pos.y + 240 };
		ImmSetCompositionWindow(hImc, &form);
		if (wParam == VK_DELETE)
		{
			this->InputDelete();
			UpdateScroll();
		}
		else if (wParam == VK_RIGHT)
		{
			if (this->SelectionEnd < this->Text.size())
			{
				this->SelectionEnd = this->SelectionEnd + 1;
				if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == false)
				{
					this->SelectionStart = this->SelectionEnd;
				}
				if (this->SelectionEnd > this->Text.size())
				{
					this->SelectionEnd = this->Text.size();
				}
				UpdateScroll();
			}
		}
		else if (wParam == VK_LEFT)
		{
			if (this->SelectionEnd > 0)
			{
				this->SelectionEnd = this->SelectionEnd - 1;
				if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == false)
				{
					this->SelectionStart = this->SelectionEnd;
				}
				if (this->SelectionEnd < 0)
				{
					this->SelectionEnd = 0;
				}
				UpdateScroll();
			}
		}
		else if (wParam == VK_HOME)
		{
			auto font = this->Font;
			std::wstring MaskText(this->Text.size(), L'*');
			auto hit = font->HitTestTextRange(MaskText, FLT_MAX, this->Height, (UINT32)this->SelectionEnd, (UINT32)0);
			this->SelectionEnd = font->HitTestTextPosition(MaskText, 0, hit[0].top + (font->FontHeight * 0.5f));
			if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == false)
			{
				this->SelectionStart = this->SelectionEnd;
			}
			if (this->SelectionEnd < 0)
			{
				this->SelectionEnd = 0;
			}
			UpdateScroll();
		}
		else if (wParam == VK_END)
		{
			std::wstring MaskText(this->Text.size(), L'*');
			auto font = this->Font;
			auto hit = font->HitTestTextRange(MaskText, FLT_MAX, this->Height, (UINT32)this->SelectionEnd, (UINT32)0);
			this->SelectionEnd = font->HitTestTextPosition(MaskText, FLT_MAX, hit[0].top + (font->FontHeight * 0.5f));
			if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == false)
			{
				this->SelectionStart = this->SelectionEnd;
			}
			if (this->SelectionEnd > this->Text.size())
			{
				this->SelectionEnd = this->Text.size();
			}
			UpdateScroll();
		}
		else if (wParam == VK_PRIOR)
		{
			auto font = this->Font;
			std::wstring MaskText(this->Text.size(), L'*');
			auto hit = font->HitTestTextRange(MaskText, FLT_MAX, this->Height, (UINT32)this->SelectionEnd, (UINT32)0);
			this->SelectionEnd = font->HitTestTextPosition(MaskText, hit[0].left, hit[0].top - this->Height);
			if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == false)
			{
				this->SelectionStart = this->SelectionEnd;
			}
			if (this->SelectionEnd < 0)
			{
				this->SelectionEnd = 0;
			}
			UpdateScroll(true);
		}
		else if (wParam == VK_NEXT)
		{
			auto font = this->Font;
			std::wstring MaskText(this->Text.size(), L'*');
			auto hit = font->HitTestTextRange(MaskText, FLT_MAX, this->Height, (UINT32)this->SelectionEnd, (UINT32)0);
			this->SelectionEnd = font->HitTestTextPosition(MaskText, hit[0].left, hit[0].top + this->Height);
			if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == false)
			{
				this->SelectionStart = this->SelectionEnd;
			}
			if (this->SelectionEnd > this->Text.size())
			{
				this->SelectionEnd = this->Text.size();
			}
			UpdateScroll(true);
		}
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyDown(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_CHAR:
	{
		wchar_t ch = (wchar_t)(wParam);
		if (ch >= 32 && ch <= 126)//input char
		{
			const wchar_t c[] = { ch,L'\0' };
			this->InputText(c);
			UpdateScroll();
		}
		else if (ch == 1)//ctrl+a
		{
			this->SelectionStart = 0;
			this->SelectionEnd = this->Text.size();
			UpdateScroll();
		}
		else if (ch == 8)//back
		{
			if (this->Text.size() > 0)
			{
				this->InputBack();
				UpdateScroll();
			}
		}
		else if (ch == 22)//crlt+v
		{
			if (OpenClipboard(this->ParentForm->Handle))
			{
				if (IsClipboardFormatAvailable(CF_TEXT))
				{
					HANDLE hClip;
					char* pBuf;
					hClip = GetClipboardData(CF_TEXT);
					pBuf = (char*)GlobalLock(hClip);
					GlobalUnlock(hClip);
					auto wc = Convert::string_to_wstring(pBuf);
					this->InputText(wc);
					UpdateScroll();
					CloseClipboard();
				}
			}
		}
		this->PostRender();
	}
	break;
	case WM_IME_COMPOSITION://imm action
	{
		if (lParam & GCS_RESULTSTR)
		{
			HIMC hIMC;
			DWORD dwSize;
			hIMC = ImmGetContext(this->ParentForm->Handle);
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);
			dwSize += sizeof(WCHAR);
			wchar_t* input = new wchar_t[dwSize];
			memset(input, 0, dwSize);
			ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, input, dwSize);
			List<wchar_t> tmp;
			for (int i = 0; i < dwSize - 2; i++)
			{
				if (input[i] > 255)
				{
					tmp.Add(input[i]);
				}
				if (!input[i]) break;
			}
			tmp.Add(L'\0');
			this->InputText(tmp.data());
			ImmReleaseContext(this->ParentForm->Handle, hIMC);
			UpdateScroll();
			this->PostRender();
		}
	}
	break;
	case WM_KEYUP://keyboard up
	{
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyUp(this, event_obj);
		this->PostRender();
	}
	break;
	}
	return true;
}