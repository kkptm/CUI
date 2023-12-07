#pragma once
#include "Control.h"
class TreeNode
{
public:
	class ID2D1Bitmap* Image = NULL;
	std::wstring Text;
	List<TreeNode*> Children;
	bool Expand;
	TreeNode(std::wstring text, ID2D1Bitmap* image = NULL)
	{
		this->Text = text;
		this->Image = image;
		Expand = false;
	}
	~TreeNode()
	{
		for (auto c : this->Children)
		{
			delete c;
		}
		this->Children.Clear();
	}
};
class TreeView : public Control
{
public:
	virtual UIClass Type();
	TreeNode* Root;
	int ScrollOffset = 0;
	TreeView(int x, int y, int width = 120, int height = 24);
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};

