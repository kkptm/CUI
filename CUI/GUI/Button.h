#pragma once
#include "Control.h"
class Button : public Control
{
public:
	virtual UIClass Type();
	D2D1_COLOR_F UnderMouseColor = Colors::SkyBlue;
	D2D1_COLOR_F CheckedColor = Colors::SteelBlue;
	float Boder = 1.0f;
	float Round = 0.25f;
	Button(std::wstring text, int x, int y, int width = 120, int height = 24);
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};
