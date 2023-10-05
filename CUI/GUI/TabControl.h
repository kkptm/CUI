#pragma once
#include "Control.h"
#include "Panel.h"
#pragma comment(lib, "Imm32.lib")
class TabPage : public Panel
{
public:
	virtual UIClass Type();
	TabPage();
	TabPage(std::wstring text);
};
class TabControl : public Control
{
public:
	virtual UIClass Type();
	D2D1_COLOR_F TitleBackColor = Colors::LightYellow3;
	D2D1_COLOR_F SelectedTitleBackColor = Colors::LightYellow1;
	int SelectIndex = 0;
	int TitleHeight = 24;
	int TitleWidth = 80;
	float Boder = 1.5f;
	TabControl(int x, int y, int width = 120, int height = 24);
	TabPage* AddPage(std::wstring name);
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};