#pragma once
#include "GridView.h"
#include "Form.h"
#include "TextBox.h"
#pragma comment(lib, "Imm32.lib")

CellValue::CellValue(std::wstring s) : std::wstring(s)
{}
CellValue::CellValue(wchar_t* s) : std::wstring(s)
{}
CellValue::CellValue(const wchar_t* s) : std::wstring(s)
{}
CellValue::CellValue(ID2D1Bitmap* img) : std::wstring(L""), Image(img)
{}
CellValue::CellValue(__int64 tag) : std::wstring(L""), Tag(tag)
{}
CellValue::CellValue(bool tag) : std::wstring(L""), Tag(tag)
{}
CellValue::CellValue(__int32 tag) : std::wstring(L""), Tag(tag)
{}
CellValue::CellValue(unsigned __int32 tag) : std::wstring(L""), Tag(tag)
{}
CellValue::CellValue(unsigned __int64 tag) : std::wstring(L""), Tag(tag)
{}
CellValue& GridViewRow::operator[](int idx)
{
	return Cells[idx];
}

GridViewColunm::GridViewColunm(std::wstring name, float width, ColumnType type, bool canEdit)
{
	Name = name;
	Width = width;
	Type = type;
	CanEdit = canEdit;
}
UIClass GridView::Type() { return UIClass::UI_GridView; }
GridView::GridView(int x, int y, int width, int height)
{
	this->Location = POINT{ x,y };
	this->Size = SIZE{ width,height };
}
GridViewRow& GridView::operator[](int idx)
{
	return Rows[idx];
}
GridViewRow& GridView::SelectedRow()
{
	static GridViewRow default_;
	if (this->SelectedRowIndex >= 0 && this->SelectedRowIndex < this->Rows.Count)
	{
		return this->Rows[this->SelectedRowIndex];
	}
	return default_;
}
std::wstring& GridView::SelectedValue()
{
	static std::wstring default_;
	if (this->SelectedRowIndex >= 0 && this->SelectedRowIndex < this->Rows.Count)
	{
		return this->Rows[this->SelectedRowIndex].Cells[SelectedColunmIndex];
	}
	return default_;
}
void GridView::Clear()
{
	this->Rows.Clear();
	this->ScrollRowPosition = 0;
}
#pragma region _GridView_
POINT GridView::GetGridViewUnderMouseItem(int x, int y, GridView* ct)
{
	float _render_width = ct->Width - 8;
	float _render_height = ct->Height;
	if (x > _render_width || y > _render_height)return { -1,-1 };
	auto font = ct->Font;
	auto head_font = HeadFont ? HeadFont : font;
	float font_height = font->FontHeight;
	float row_height = font_height + 2.0f;
	if (RowHeight != 0.0f)
	{
		row_height = RowHeight;
	}
	float head_font_height = head_font->FontHeight;
	float head_height = ct->HeadHeight == 0.0f ? head_font_height : ct->HeadHeight;
	if (y < head_height)
	{
		return { -1,-1 };
	}
	unsigned int s_x = ct->ScrollColumnPosition;
	unsigned int s_y = ct->ScrollRowPosition;
	float yf = ct->HeadHeight == 0.0f ? row_height : ct->HeadHeight;
	float xf = 0.0f;
	int xindex = -1;
	int yindex = -1;
	for (; s_x < ct->Colunms.Count; s_x++)
	{
		float c_width = ct->Colunms[s_x].Width;
		if (c_width + xf > _render_width)
		{
			c_width = _render_width - xf;
		}
		if (xf<x && xf + c_width>x)
		{
			xindex = s_x;
			break;
		}
		xf += ct->Colunms[s_x].Width;
		if (xf > _render_width)
		{
			break;
		}
	}
	if (((y - head_height) / row_height) + s_y < ct->Rows.Count)
	{
		yindex = ((y - head_height) / row_height) + s_y;
	}
	return { xindex,yindex };
}
D2D1_RECT_F GridView::GetGridViewScrollBlockRect(GridView* ct)
{
	auto absloc = ct->AbsLocation;
	auto size = ct->Size;
	float _render_width = ct->Width - 8;
	float _render_height = ct->Height;
	auto font = ct->Font;
	auto head_font = HeadFont ? HeadFont : font;
	float font_height = font->FontHeight;
	float row_height = font_height + 2.0f;
	if (RowHeight != 0.0f)
	{
		row_height = RowHeight;
	}
	float head_font_height = head_font->FontHeight;
	float head_height = ct->HeadHeight == 0.0f ? head_font_height : ct->HeadHeight;
	float render_items_height = _render_height - head_height;
	int render_items_count = render_items_height / row_height;
	if (render_items_count < ct->Rows.Count)
	{
		float scroll_block_height = (float)ct->Height * (float)render_items_count / (float)ct->Rows.Count;
		float scroll_block_top = ((float)ct->ScrollRowPosition / ((float)ct->Rows.Count)) * (float)ct->Height;
		return { absloc.x + (ct->Width - 8.0f), absloc.y + scroll_block_top, 8.0f, scroll_block_height };
	}
	return { 0,0,0,0 };
}
int GridView::GetGridViewRenderRowCount(GridView* ct)
{
	float _render_height = ct->Height;
	float font_height = (ct->Font)->FontHeight;
	float row_height = font_height + 2.0f;
	if (RowHeight != 0.0f)
	{
		row_height = RowHeight;
	}
	auto font = ct->Font;
	auto head_font = HeadFont ? HeadFont : font;
	float head_font_height = head_font->FontHeight;
	float head_height = ct->HeadHeight == 0.0f ? head_font_height : ct->HeadHeight;
	_render_height -= head_height;
	return (int)(_render_height / row_height);
}
#pragma endregion
void GridView::DrawScroll()
{
	auto d2d = this->Render;
	auto abslocation = this->AbsLocation;
	auto font = this->Font;
	auto size = this->ActualSize();
	if (this->Rows.Count > 0)
	{
		float _render_width = this->Width - 8;
		float _render_height = this->Height;
		float font_height = font->FontHeight;
		float row_height = font_height + 2.0f;
		if (RowHeight != 0.0f)
		{
			row_height = RowHeight;
		}
		auto head_font = HeadFont ? HeadFont : font;
		float head_font_height = head_font->FontHeight;
		float head_height = this->HeadHeight == 0.0f ? head_font_height : this->HeadHeight;
		float render_items_height = _render_height - head_height;
		int render_items_count = render_items_height / row_height;
		if (render_items_count < this->Rows.Count)
		{
			int render_count = GetGridViewRenderRowCount(this);
			int max_scroll = this->Rows.Count - render_count;
			float scroll_block_height = ((float)render_count / (float)this->Rows.Count) * (float)this->Height;
			if (scroll_block_height < this->Height * 0.1)scroll_block_height = this->Height * 0.1;
			float scroll_block_move_space = this->Height - scroll_block_height;
			float yt = scroll_block_height * 0.5f;
			float yb = this->Height - (scroll_block_height * 0.5f);
			float per = (float)this->ScrollRowPosition / (float)max_scroll;
			float scroll_tmp_y = per * scroll_block_move_space;
			float scroll_block_top = scroll_tmp_y;
			d2d->FillRoundRect(abslocation.x + (this->Width - 8.0f), abslocation.y, 8.0f, this->Height, this->ScrollBackColor, 4.0f);
			d2d->FillRoundRect(abslocation.x + (this->Width - 8.0f), abslocation.y + scroll_block_top, 8.0f, scroll_block_height, this->ScrollForeColor, 4.0f);
		}
	}
}
void GridView::SetScrollByPos(float yof)
{
	auto d2d = this->Render;
	auto abslocation = this->AbsLocation;
	auto font = this->Font;
	auto size = this->ActualSize();
	int render_count = GetGridViewRenderRowCount(this);
	int max_scroll = this->Rows.Count - render_count;
	if (this->Rows.Count > 0)
	{
		float _render_width = this->Width - 8;
		float _render_height = this->Height;
		float font_height = font->FontHeight;
		float row_height = font_height + 2.0f;
		if (RowHeight != 0.0f)
		{
			row_height = RowHeight;
		}
		auto head_font = HeadFont ? HeadFont : font;
		float head_font_height = head_font->FontHeight;
		float head_height = this->HeadHeight == 0.0f ? head_font_height : this->HeadHeight;
		float render_items_height = _render_height - head_height;
		int render_items_count = render_items_height / row_height;
		if (render_items_count < this->Rows.Count)
		{
			float scroll_block_height = ((float)render_count / (float)this->Rows.Count) * (float)this->Height;
			if (scroll_block_height < this->Height * 0.1)scroll_block_height = this->Height * 0.1;
			float yt = scroll_block_height * 0.5f;
			float yb = this->Height - (scroll_block_height * 0.5f);
			float per = (yof - yt) / (yb - yt);
			this->ScrollRowPosition = max_scroll * per;
		}
	}
	if (this->ScrollRowPosition < 0)this->ScrollRowPosition = 0;
	if (this->ScrollRowPosition > this->Rows.Count - render_count)this->ScrollRowPosition = this->Rows.Count - render_count;
	this->ScrollChanged(this);
}
void GridView::Update()
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
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, this->BackColor);
		if (this->Image)
		{
			this->RenderImage();
		}
		auto font = this->Font;
		auto head_font = HeadFont ? HeadFont : font;
		{
			float _render_width = this->Width - 8;
			float _render_height = this->Height;
			float font_height = font->FontHeight;
			float head_font_height = head_font->FontHeight;
			float row_height = font_height + 2.0f;
			if (RowHeight != 0.0f)
			{
				row_height = RowHeight;
			}
			float text_top = (row_height - font_height) * 0.5f;
			if (text_top < 0) text_top = 0;
			unsigned int s_x = this->ScrollColumnPosition;
			unsigned int s_y = this->ScrollRowPosition;
			float head_height = this->HeadHeight == 0.0f ? head_font_height : this->HeadHeight;
			float yf = head_height;
			float xf = 0.0f;
			int i = s_x;
			for (; i < this->Colunms.Count; i++)
			{
				float c_width = this->Colunms[i].Width;
				if (c_width + xf > _render_width)
				{
					c_width = _render_width - xf;
				}
				auto ht = d2d->GetTextSize(this->Colunms[i].Name, head_font);
				float draw_x_offset = (c_width - ht.width) / 2.0f;
				if (draw_x_offset < 0)draw_x_offset = 0;
				float draw_y_offset = (head_height - head_font_height) / 2.0f;
				if (draw_y_offset < 0)draw_y_offset = 0;
				d2d->PushDrawRect(abslocation.x + xf, abslocation.y, c_width, head_height);
				{
					d2d->FillRect(abslocation.x + xf, abslocation.y, c_width, head_height, this->HeadBackColor);
					d2d->DrawRect(abslocation.x + xf, abslocation.y, c_width, head_height, this->HeadForeColor, 2.f);
					d2d->DrawString(this->Colunms[i].Name,
						abslocation.x + xf + draw_x_offset,
						abslocation.y + draw_y_offset,
						this->HeadForeColor, head_font);
				}
				d2d->PopDrawRect();
				xf += this->Colunms[i].Width;
				if (xf > _render_width)
				{
					break;
				}
			}
			xf = 0;
			i = 0;
			for (int r = s_y; r < this->Rows.Count && i < (int)(_render_height / row_height); r++, i++)
			{
				GridViewRow& row = this->Rows[r];
				float xf = 0.0f;
				for (int c = s_x; c < this->Colunms.Count; c++)
				{
					{
						float c_width = this->Colunms[c].Width;
						if (c_width + xf > _render_width)
						{
							c_width = _render_width - xf;
						}
						float _r_height = row_height;
						d2d->PushDrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height);
						{
							switch (this->Colunms[c].Type)
							{
							case ColumnType::Text:
							{
								float _size = c_width < row_height ? c_width : row_height;
								if (c == this->SelectedColunmIndex && r == this->SelectedRowIndex)
								{
									d2d->FillRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->SelectedItemBackColor);
									d2d->DrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->SelectedItemForeColor,
										r == this->UnderMouseRowIndex ? 1.0f : 0.5f);
									if (row.Cells.Count > c)
										d2d->DrawString(row.Cells[c],
											abslocation.x + xf + 1.0f,
											abslocation.y + yf + text_top,
											this->SelectedItemForeColor, font);
								}
								else if (c == this->UnderMouseColunmIndex && r == this->UnderMouseRowIndex)
								{
									d2d->FillRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->UnderMouseItemBackColor);
									d2d->DrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->UnderMouseItemForeColor,
										r == this->UnderMouseRowIndex ? 1.0f : 0.5f);
									if (row.Cells.Count > c)
										d2d->DrawString(row.Cells[c],
											abslocation.x + xf + 1.0f,
											abslocation.y + yf + text_top,
											this->UnderMouseItemForeColor, font);
								}
								else
								{
									d2d->DrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->ForeColor,
										r == this->UnderMouseRowIndex ? 1.0f : 0.5f);
									if (row.Cells.Count > c)
										d2d->DrawString(row.Cells[c],
											abslocation.x + xf + 1.0f,
											abslocation.y + yf + text_top,
											this->ForeColor, font);
								}
							}
							break;
							case ColumnType::Image:
							{
								float _size = c_width < row_height ? c_width : row_height;
								float left = (c_width - _size) / 2.0f;
								float top = (row_height - _size) / 2.0f;
								if (c == this->UnderMouseColunmIndex && r == this->UnderMouseRowIndex)
								{
									d2d->FillRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->UnderMouseItemBackColor);
									d2d->DrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->UnderMouseItemForeColor,
										r == this->UnderMouseRowIndex ? 1.0f : 0.5f);
									if (row.Cells.Count > c)
									{
										d2d->DrawBitmap(row.Cells[c].Image,
											abslocation.x + xf + left,
											abslocation.y + yf + top,
											_size, _size
										);
									}
								}
								else
								{
									d2d->DrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->ForeColor,
										r == this->UnderMouseRowIndex ? 1.0f : 0.5f);
									if (row.Cells.Count > c)
									{
										d2d->DrawBitmap(row.Cells[c].Image,
											abslocation.x + xf + left,
											abslocation.y + yf + top,
											_size, _size
										);
									}
								}
							}
							break;
							case ColumnType::Check:
							{
								float _size = c_width < row_height ? c_width : row_height;
								if (_size > 24)_size = 24;
								float left = (c_width - _size) / 2.0f;
								float top = (row_height - _size) / 2.0f;
								float _rsize = _size;
								if (c == this->UnderMouseColunmIndex && r == this->UnderMouseRowIndex)
								{
									d2d->FillRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->UnderMouseItemBackColor);
									d2d->DrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->UnderMouseItemForeColor,
										r == this->UnderMouseRowIndex ? 1.0f : 0.5f);
									if (row.Cells.Count > c)
									{
										d2d->DrawRect(
											abslocation.x + xf + left + (_rsize * 0.2),
											abslocation.y + yf + top + (_rsize * 0.2),
											_rsize * 0.6, _rsize * 0.6,
											this->ForeColor);
										if (row.Cells[c].Tag)
										{
											d2d->FillRect(
												abslocation.x + xf + left + (_rsize * 0.35),
												abslocation.y + yf + top + (_rsize * 0.35),
												_rsize * 0.3, _rsize * 0.3,
												this->ForeColor);
										}
									}
								}
								else
								{
									d2d->DrawRect(abslocation.x + xf, abslocation.y + yf, c_width, _r_height, this->ForeColor,
										r == this->UnderMouseRowIndex ? 1.0f : 0.5f);
									if (row.Cells.Count > c)
									{
										d2d->DrawRect(
											abslocation.x + xf + left + (_rsize * 0.2),
											abslocation.y + yf + top + (_rsize * 0.2),
											_rsize * 0.6, _rsize * 0.6,
											this->ForeColor);
										if (row.Cells[c].Tag)
										{
											d2d->FillRect(
												abslocation.x + xf + left + (_rsize * 0.35),
												abslocation.y + yf + top + (_rsize * 0.35),
												_rsize * 0.3, _rsize * 0.3,
												this->ForeColor);
										}
									}
								}
							}
							break;
							default:
								break;
							}
						}
						d2d->PopDrawRect();
					}
					xf += this->Colunms[c].Width;
					if (xf > _render_width)
					{
						break;
					}
				}
				yf += row_height;
			}
			d2d->PushDrawRect(
				(float)abslocation.x,
				(float)abslocation.y,
				(float)size.cx,
				(float)size.cy);
			{
				if (this->ParentForm->UnderMouse == this)
				{
					d2d->DrawRect(abslocation.x, abslocation.y, size.cx, size.cy, this->BolderColor, 4);
				}
				else
				{
					d2d->DrawRect(abslocation.x, abslocation.y, size.cx, size.cy, this->BolderColor, 2);
				}
			}
			d2d->PopDrawRect();
			this->DrawScroll();
		}
		d2d->DrawRect(abslocation.x, abslocation.y, size.cx, size.cy, this->BolderColor, this->Boder);
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
	if (this->Count == 0)
	{
		this->AddControl(new TextBox(L"", 10, 10))->Visable = true;
		this->get(0)->Tag = 0xFFFFFFFFFFFFFFFF;
		this->get(0)->OnTextChanged += [](class Control* sender, std::wstring, std::wstring)
			{
				GridView* gd = (GridView*)sender->Parent;
				int xx = sender->Tag >> 32;
				int yy = sender->Tag & 0xffffffff;
				if (xx >= 0 && yy >= 0)
				{
					std::wstring str = sender->Text.c_str();
					gd->Rows[yy].Cells[xx] = str;
				}

			};
		this->get(0)->OnKeyDown += [](class Control* sender, KeyEventArgs e)
			{
				GridView* gd = (GridView*)sender->Parent;
				if (e.KeyData == Keys::Return)
				{
					if (gd->SelectedRowIndex < gd->Rows.Count - 1)
					{
						TextBox* tb = (TextBox*)sender;
						gd->ChangeEditionSelected(gd->SelectedColunmIndex, gd->SelectedRowIndex + 1);
						tb->SelectionStart = 0;
						tb->SelectionEnd = tb->Text.length();
						if (tb->Bottom > gd->Height)
						{
							gd->ScrollRowPosition += 1;
						}
					}
				}
			};
	}
	TextBox* c = (TextBox*)this->get(0);
	if (this->UpdateEdit() && (isSelected || this->ParentForm->Selected ==c))
	{
		c->Update();
	}
}

bool GridView::UpdateEdit()
{
	TextBox* c = (TextBox*)this->get(0);
	if (this->SelectedColunmIndex >= 0 && 
		this->Colunms[this->SelectedColunmIndex].Type == ColumnType::Text &&
		this->Colunms[this->SelectedColunmIndex].CanEdit)
	{
		if (this->SelectedRowIndex >= 0)
		{

			int topIndex = this->ScrollRowPosition;
			int drawIndex = this->SelectedRowIndex - topIndex;
			auto d2d = this->Render;
			auto font = this->Font;
			float font_height = font->FontHeight;
			float row_height = font_height + 2.0f;

			float renderLeft = 0.0f;
			for (int i = 0; i < this->SelectedColunmIndex; i++)
			{
				renderLeft += this->Colunms[i].Width;
			}
			auto head_font = HeadFont ? HeadFont : font;
			float head_height = this->HeadHeight == 0.0f ? head_font->FontHeight : this->HeadHeight;
			float rendertop = head_height + (row_height * drawIndex);
			if (rendertop >= head_height && rendertop <= this->Height)
			{
				c->Location = POINT{ (int)renderLeft ,(int)rendertop };
				c->Size = SIZE{ (int)this->Colunms[this->SelectedColunmIndex].Width ,(int)row_height };
				c->Visable = true;
				return true;
			}
			return false;
		}
	}
	c->Visable = false;
	return false;
}
void GridView::AutoSizeColumn(int col)
{
	if (this->Colunms.Count > col)
	{
		auto font = this->Font;
		float font_height = font->FontHeight;
		float row_height = font_height + 2.0f;
		if (RowHeight != 0.0f)
		{
			row_height = RowHeight;
		}
		auto& column = this->Colunms[col];
		column.Width = 10.0f;
		for (int i = 0; i < this->Rows.Count; i++)
		{
			auto& r = this->Rows[i];
			if (r.Cells.Count > col)
			{
				if (this->Colunms[col].Type == ColumnType::Text)
				{
					auto width = font->GetTextSize(r.Cells[col].c_str()).width;
					if (column.Width < width)
					{
						column.Width = width;
					}
				}
				else
				{
					column.Width = row_height;
				}
			}
		}
	}
}

void GridView::ChangeEditionSelected(int col,int row)
{
	if (this->get(0)->Visable)
	{
		int oldx = this->get(0)->Tag >> 32;
		int oldy = this->get(0)->Tag & 0xffffffff;
		if (oldx >= 0 && oldy >= 0)
		{
			std::wstring str = this->get(0)->Text.c_str();
			this->Rows[oldy].Cells[oldx] = str;
		}
	}
	if (this->Colunms[col].Type == ColumnType::Text && this->Colunms[col].CanEdit)
	{
		TextBox* tb = (TextBox*)this->get(0);
		tb->Visable = true;
		tb->Tag = (ULONG64)col << 32 | (ULONG64)row;
		tb->Text = this->Rows[row].Cells[col];
		tb->SelectionStart = 0;
		this->ParentForm->Selected = tb;
	}
	this->SelectedColunmIndex = col;
	this->SelectedRowIndex = row;
	this->SelectionChanged(this);
}
bool GridView::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
{
	if (!this->Enable || !this->Visable) return true;
	bool HitEdit = false;
	POINT input_location = { 0,0 };
	TextBox* c = (TextBox*)this->get(0);
	if (c)
	{
		input_location = c->Location;
		auto size = c->ActualSize();
		HitEdit =
			(xof >= input_location.x &&
				yof >= input_location.y &&
				xof <= (input_location.x + size.cx) &&
				yof <= (input_location.y + size.cy)) || c->Visable;
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
	case WM_MOUSEWHEEL://mouse wheel
	{
		bool need_update = false;
		if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			auto d2d = this->Render;
			auto font = this->Font;
			float font_height = font->FontHeight;
			float row_height = font_height + 2.0f;
			if (RowHeight != 0.0f)
			{
				row_height = RowHeight;
			}
			float head_height = this->HeadHeight == 0.0f ? row_height : this->HeadHeight;
			int pre_render_item_count = (this->Rows.Count - this->ScrollRowPosition) - 1;
			float pre_render_items_height = (pre_render_item_count * row_height) + head_height;
			if (pre_render_items_height + row_height >= this->Height)
			{
				need_update = true;
				this->ScrollRowPosition += 1;
				this->ScrollChanged(this);
			}
		}
		else
		{
			if (this->ScrollRowPosition > 0)
			{
				need_update = true;
				this->ScrollRowPosition -= 1;
				this->ScrollChanged(this);
			}
		}
		auto undermouseindex = GetGridViewUnderMouseItem(xof, yof, this);
		this->UnderMouseColunmIndex = undermouseindex.x;
		this->UnderMouseRowIndex = undermouseindex.y;
		MouseEventArgs event_obj = MouseEventArgs(MouseButtons::None, 0, xof, yof, GET_WHEEL_DELTA_WPARAM(wParam));
		this->OnMouseWheel(this, event_obj);
		if (need_update)
		{
			this->PostRender();
		}
	}
	break;
	case WM_MOUSEMOVE://mouse move
	{
		this->ParentForm->UnderMouse = this;
		bool need_update = false;
		if (this->InScroll)
		{
			need_update = true;
			SetScrollByPos(yof);
		}
		else
		{
			auto undermouseindex = GetGridViewUnderMouseItem(xof, yof, this);
			if (this->UnderMouseColunmIndex != undermouseindex.x ||
				this->UnderMouseRowIndex != undermouseindex.y)
			{
				need_update = true;
			}
			this->UnderMouseColunmIndex = undermouseindex.x;
			this->UnderMouseRowIndex = undermouseindex.y;
		}
		MouseEventArgs event_obj = MouseEventArgs(MouseButtons::None, 0, xof, yof, HIWORD(wParam));
		this->OnMouseMove(this, event_obj);
		if (need_update)
		{
			this->PostRender();
		}
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
			if (xof < this->Width - 8)
			{
				auto undermouseindex = GetGridViewUnderMouseItem(xof, yof, this);
				if (undermouseindex.y < this->Rows.Count && undermouseindex.x < this->Colunms.Count)
				{
					if (this->Colunms[undermouseindex.x].Type == ColumnType::Check)
					{
						this->Rows[undermouseindex.y].Cells[undermouseindex.x].Tag = __int64(this->Rows[undermouseindex.y].Cells[undermouseindex.x].Tag == 0);
						this->OnGridViewCheckStateChanged(this, undermouseindex.x, undermouseindex.y, this->Rows[undermouseindex.y].Cells[undermouseindex.x].Tag != 0);
					}
					else
					{
						if (this->SelectedColunmIndex != undermouseindex.x || this->SelectedRowIndex != undermouseindex.y)
						{
							if (this->get(0)->Visable)
							{
								int oldx = this->get(0)->Tag >> 32;
								int oldy = this->get(0)->Tag & 0xFFFFFFFF;
								if (oldx >= 0 && oldy >= 0)
								{
									std::wstring str = this->get(0)->Text.c_str();
									this->Rows[oldy].Cells[oldx] = str;
								}
							}
							if (this->Colunms[undermouseindex.x].Type == ColumnType::Text && this->Colunms[undermouseindex.x].CanEdit &&
								undermouseindex.x >= 0 && undermouseindex.y >= 0
								)
							{
								TextBox* tb = (TextBox*)this->get(0);
								tb->Visable = true;
								tb->Tag = (ULONG64)undermouseindex.x << 32 | (ULONG64)undermouseindex.y;
								tb->Text = this->Rows[undermouseindex.y].Cells[undermouseindex.x];
								tb->SelectionStart = 0;
								this->ParentForm->Selected = tb;
							}
							this->SelectedColunmIndex = undermouseindex.x;
							this->SelectedRowIndex = undermouseindex.y;
							this->SelectionChanged(this);
						}
					}
				}
			}
			else
			{
				this->InScroll = true;
				SetScrollByPos(yof);
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
		this->InScroll = false;
		if (WM_LBUTTONUP == message && this->ParentForm->Selected == this)
		{
			MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
			this->OnMouseClick(this, event_obj);
		}
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseUp(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_LBUTTONDBLCLK://mouse double click
	{
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseDoubleClick(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_KEYDOWN://keyboard down
	{
		if (wParam == VK_RIGHT)
		{
			SelectedColunmIndex += 1;
			if (SelectedColunmIndex >= this->Colunms.Count)SelectedColunmIndex = this->Colunms.Count - 1;
		}
		else if (wParam == VK_LEFT)
		{
			SelectedColunmIndex -= 1;
			if (SelectedColunmIndex < 0)SelectedColunmIndex = 0;
		}
		else if (wParam == VK_DOWN)
		{
			SelectedRowIndex += 1;
			if (SelectedRowIndex >= this->Rows.Count)
				SelectedRowIndex = this->Rows.Count - 1;
		}
		else if (wParam == VK_UP)
		{
			SelectedRowIndex -= 1;
			if (SelectedRowIndex < 0)SelectedRowIndex = 0;
		}
		int rc = GetGridViewRenderRowCount(this) - 1;
		if (SelectedRowIndex < this->ScrollRowPosition)
			this->ScrollRowPosition = SelectedRowIndex;
		if (SelectedRowIndex > this->ScrollRowPosition + rc)
			this->ScrollRowPosition += 1;
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyDown(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_KEYUP://keyboard up
	{
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyUp(this, event_obj);
		if (HitEdit)
			c->ProcessMessage(message, wParam, lParam, xof - input_location.x, yof - input_location.y);
	}
	break;
	}
	if (HitEdit)
		c->ProcessMessage(message, wParam, lParam, xof - input_location.x, yof - input_location.y);
	return true;
}

