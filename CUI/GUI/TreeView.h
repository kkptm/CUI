#pragma once
#include "Control.h"
class TreeNode
{
public:
	class ID2D1Bitmap* Image = NULL;
	std::wstring Text;
	List<TreeNode*> Children;
	bool Expand = false; 
	TreeNode(std::wstring text, ID2D1Bitmap* image = NULL);
	~TreeNode();
	int UnfoldedCount();
};
class TreeView : public Control
{
private:
	bool isDraggingScroll;
	void UpdateScrollDrag(float posY);
	void DrawScroll();
public:
	virtual UIClass Type();
	TreeNode* Root;
	TreeNode* SelectedNode;
	int MaxRenderItems = 0;
	int ScrollIndex = 0;
	D2D1_COLOR_F ScrollBackColor = Colors::LightGray;
	D2D1_COLOR_F ScrollForeColor = Colors::DimGrey;
	D2D1_COLOR_F SelectedBackColor = Colors::Blue;
	D2D1_COLOR_F SelectedForeColor = Colors::White;
	ScrollChangedEvent ScrollChanged;
	SelectionChangedEvent SelectionChanged;
	TreeView(int x, int y, int width = 120, int height = 24);
	~TreeView();
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};

