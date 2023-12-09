#pragma once
#include "Control.h"
class TreeNode
{
public:
	class ID2D1Bitmap* Image = NULL;
	std::wstring Text;
	List<TreeNode*> Children;
	bool Expand = false;;
	TreeNode(std::wstring text, ID2D1Bitmap* image = NULL);
	~TreeNode();
	int UnfoldedCount();
};
class TreeView : public Control
{
private:
	bool InScroll = false;
public:
	virtual UIClass Type();
	TreeNode* Root;
	TreeNode* SelectedNode;
	int MaxRenderItems = 0;
	int ScrollOffset = 0;
	D2D1_COLOR_F ScrollBackColor = Colors::LightGray;
	D2D1_COLOR_F ScrollForeColor = Colors::DimGrey;
	ScrollChangedEvent ScrollChanged;
	SelectionChangedEvent SelectionChanged;
	TreeView(int x, int y, int width = 120, int height = 24);
	~TreeView();
	void DrawScroll();
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};

