#include "TreeView.h"
#include "Form.h"
static void renderNodes(TreeView* tree, Graphics* d2d, float x, float y, float w, float h, float itemHeight, int scrollIndex, int& currindex, int sunLevel, List<TreeNode*>& Children)
{
	float baseLeft = sunLevel * itemHeight + 3.5f + x;
	for (auto c : Children)
	{
		int renderIndex = currindex - scrollIndex;
		float renderTop = itemHeight * renderIndex;

		if (renderTop >= 0.0f && renderTop < h)
		{
			float renderLeft = baseLeft;

			float exTop = renderTop + (itemHeight * 0.2f) + y;
			auto foreColor = (c == tree->SelectedNode) ? Colors::White : tree->ForeColor;
			if (c == tree->SelectedNode)
			{
				d2d->FillRect(x, renderTop + y, w, itemHeight, Colors::Blue);
			}
			if (c->Children.Count > 0)
			{
				d2d->DrawRect(renderLeft, exTop, itemHeight * 0.6f, itemHeight * 0.6f, foreColor);

				if (c->Expand)
				{
					// -
					d2d->DrawLine(
						{ renderLeft + (itemHeight * 0.2f), exTop + (itemHeight * 0.3f) },
						{ renderLeft + (itemHeight * 0.4f), exTop + (itemHeight * 0.3f) },
						foreColor);
				}
				else
				{
					// +
					d2d->DrawLine(
						{ renderLeft + (itemHeight * 0.2f), exTop + (itemHeight * 0.3f) },
						{ renderLeft + (itemHeight * 0.4f), exTop + (itemHeight * 0.3f) },
						foreColor);
					d2d->DrawLine(
						{ renderLeft + (itemHeight * 0.3f), exTop + (itemHeight * 0.2f) },
						{ renderLeft + (itemHeight * 0.3f), exTop + (itemHeight * 0.4f) },
						foreColor);
				}

				d2d->DrawString(c->Text, renderLeft + (itemHeight * 0.8f), renderTop + y, foreColor);
			}
			else
			{
				d2d->DrawString(c->Text, renderLeft, renderTop + y, foreColor, tree->Font ? tree->Font : d2d->DefaultFontObject);
			}
		}

		currindex += 1;

		if (c->Expand)
		{
			renderNodes(tree, d2d, x, y, w, h, itemHeight, scrollIndex, currindex, sunLevel + 1, c->Children);
		}
	}
}

static TreeNode* findNode(float posX, float posY, float w, float h, float itemHeight, int scrollIndex, int& currindex, int sunLevel, List<TreeNode*>& Children, bool& isHitEx)
{
	for (auto c : Children)
	{
		int renderIndex = currindex - scrollIndex;
		float currTop = itemHeight * renderIndex;
		float currBottom = currTop + itemHeight;
		if (currTop >= 0.0f && currTop < h)
		{
			if (posY >= currTop && posY <= currBottom)
			{
				float exLeft = (sunLevel * itemHeight) + 3.5f;
				if (posX >= exLeft && posX <= (exLeft + itemHeight))
				{
					isHitEx = true;
				}
				else
				{
					isHitEx = false;
				}
				return c;
			}
		}
		currindex += 1;
		if (c->Expand)
		{
			auto result = findNode(posX, posY, w, h, itemHeight, scrollIndex, currindex, sunLevel + 1, c->Children, isHitEx);
			if (result)
			{
				return result;
			}
		}
	}

	return NULL;
}

TreeNode::TreeNode(std::wstring text, ID2D1Bitmap* image)
{
	this->Text = text;
	this->Image = image;
	this->Expand = false;
	this->Children = List<TreeNode*>();
}
TreeNode::~TreeNode()
{
	for (auto c : this->Children)
	{
		delete c;
	}
	this->Children.Clear();
}
int TreeNode::UnfoldedCount()
{
	int count = 1;
	for (auto c : this->Children)
	{
		if (c->Expand)
		{
			count += c->UnfoldedCount();
		}
	}
	return count;
}
UIClass TreeView::Type() { return UIClass::UI_TreeView; }

TreeView::TreeView(int x, int y, int width, int height)
{
	this->Location = POINT{ x,y };
	this->Size = SIZE{ width,height };
	this->Root = new TreeNode(L"");
	this->SelectedNode = NULL;
}
TreeView::~TreeView()
{
	delete this->Root;
}
void TreeView::DrawScroll()
{
	auto d2d = this->Render;
	auto abslocation = this->AbsLocation;
	auto font = this->Font ? this->Font : d2d->DefaultFontObject;
	auto size = this->ActualSize();
	if (this->MaxRenderItems > 0)
	{
		float _render_width = this->Width - 8;
		float _render_height = this->Height;
		float font_height = font->FontHeight;
		float render_items_height = _render_height;
		int render_items_count = render_items_height / font_height;
		if (render_items_count < this->MaxRenderItems)
		{
			int render_count = (_render_height / font_height) + 1;
			int max_scroll = this->MaxRenderItems - render_count;
			float scroll_block_height = ((float)render_count / (float)this->MaxRenderItems) * (float)this->Height;
			if (scroll_block_height < this->Height * 0.1)scroll_block_height = this->Height * 0.1;
			float scroll_block_move_space = this->Height - scroll_block_height;
			float yt = scroll_block_height * 0.5f;
			float yb = this->Height - (scroll_block_height * 0.5f);
			float per = (float)this->ScrollOffset / (float)max_scroll;
			float scroll_tmp_y = per * scroll_block_move_space;
			float scroll_block_top = scroll_tmp_y;
			d2d->FillRoundRect(abslocation.x + (this->Width - 8.0f), abslocation.y, 8.0f, this->Height, this->ScrollBackColor, 4.0f);
			d2d->FillRoundRect(abslocation.x + (this->Width - 8.0f), abslocation.y + scroll_block_top, 8.0f, scroll_block_height, this->ScrollForeColor, 4.0f);
		}
	}
}
void TreeView::Update()
{
	if (this->IsVisual == false)return;
	bool isUnderMouse = this->ParentForm->UnderMouse == this;
	auto d2d = this->Render;
	auto font = this->Font ? this->Font : d2d->DefaultFontObject;
	auto abslocation = this->AbsLocation;
	auto size = this->ActualSize();
	auto absRect = this->AbsRect;
	bool isSelected = this->ParentForm->Selected == this;
	d2d->PushDrawRect(absRect.left, absRect.top, absRect.right - absRect.left, absRect.bottom - absRect.top);
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, this->BackColor);
		if (this->Image)
		{
			this->RenderImage();
		}
		//draw entities
		{
			int curr = 0;
			renderNodes(this,d2d, abslocation.x, abslocation.y, size.cx, size.cy, font->FontHeight, ScrollOffset, curr, 0, this->Root->Children);
			this->MaxRenderItems = curr;
			this->DrawScroll();
			printf("%d\n",curr);

		}
		if (!this->Enable)
		{
			d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
		}
	}
	if (!this->Enable)
	{
		d2d->FillRect(abslocation.x, abslocation.y, size.cx, size.cy, { 1.0f ,1.0f ,1.0f ,0.5f });
	}
	d2d->PopDrawRect();
}
bool TreeView::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof)
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
		bool need_update = false;
		if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			auto d2d = this->Render;
			auto font = this->Font ? this->Font : d2d->DefaultFontObject;
			float font_height = font->FontHeight;
			int pre_render_item_count = (this->MaxRenderItems - this->ScrollOffset) - 1;
			float pre_render_items_height = (pre_render_item_count * font_height);
			if (pre_render_items_height + font_height >= this->Height)
			{
				need_update = true;
				this->ScrollOffset += 1;
				this->ScrollChanged(this);
			}
		}
		else
		{
			if (this->ScrollOffset > 0)
			{
				need_update = true;
				this->ScrollOffset -= 1;
				this->ScrollChanged(this);
			}
		}
		MouseEventArgs event_obj = MouseEventArgs(MouseButtons::None, 0, xof, yof, GET_WHEEL_DELTA_WPARAM(wParam));
		this->OnMouseWheel(this, event_obj);
	}
	break;
	case WM_MOUSEMOVE://mouse move
	{
		this->ParentForm->UnderMouse = this;
		if ((GetKeyState(VK_LBUTTON) & 0x8000) && this->ParentForm->Selected == this)
		{
			auto font = this->Font ? this->Font : this->Render->DefaultFontObject;
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
			auto font = this->Font ? this->Font : this->Render->DefaultFontObject;
			auto abslocation = this->AbsLocation;
			auto size = this->ActualSize();
			int curr = 0;
			bool isHit = false;
			auto node = findNode(xof, yof, size.cx, size.cy, font->FontHeight, ScrollOffset, curr, 0, this->Root->Children, isHit);
			if (node)
			{
				if (isHit)
				{
					node->Expand = !node->Expand;
				}
				else
				{
					bool isChanged = this->SelectedNode != node;
					this->SelectedNode = node;
					if(isChanged)this->SelectionChanged(this);
				}
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
		MouseEventArgs event_obj = MouseEventArgs(FromParamToMouseButtons(message), 0, xof, yof, HIWORD(wParam));
		this->OnMouseUp(this, event_obj);
		this->PostRender();
	}
	break;
	case WM_LBUTTONDBLCLK://mouse double click
	{
		this->ParentForm->Selected = this;
		auto font = this->Font ? this->Font : this->Render->DefaultFontObject;
		auto abslocation = this->AbsLocation;
		auto size = this->ActualSize();
		int curr = 0;
		bool isHit = false;
		auto node = findNode(xof, yof, size.cx, size.cy, font->FontHeight, ScrollOffset, curr, 0, this->Root->Children, isHit);
		if (node)
		{
			node->Expand = !node->Expand;
			bool isChanged = this->SelectedNode != node;
			this->SelectedNode = node;
			if (isChanged)this->SelectionChanged(this);
		}
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
		KeyEventArgs event_obj = KeyEventArgs((Keys)(wParam | 0));
		this->OnKeyDown(this, event_obj);
		this->PostRender();
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