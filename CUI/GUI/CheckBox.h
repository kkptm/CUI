#pragma once
#include "Control.h"
class CheckBox : public Control
{
	float last_width = 0.0f;
public:
	virtual UIClass Type();
	D2D1_COLOR_F UnderMouseColor = Colors::DarkSlateGray;
	float Boder = 1.5f;
	CheckBox(std::wstring text, int x, int y);
	SIZE ActualSize() override;
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};
