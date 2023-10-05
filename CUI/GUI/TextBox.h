#pragma once
#include "Control.h"
#pragma comment(lib, "Imm32.lib")
class TextBox : public Control
{
public:
	virtual UIClass Type();
	D2D1_SIZE_F textSize = { 0,0 };
	D2D1_COLOR_F UnderMouseColor = Colors::White;
	D2D1_COLOR_F SelectedBackColor = Colors::Blue;
	D2D1_COLOR_F SelectedForeColor = Colors::White;
	D2D1_COLOR_F FocusedColor = Colors::White;
	D2D1_COLOR_F ScrollBackColor = Colors::LightGray;
	D2D1_COLOR_F ScrollForeColor = Colors::DimGrey;
	int SelectionStart = 0;
	int SelectionEnd = 0;
	float Boder = 1.0f;
	float OffsetX = 0.0f;
	float TextMargin = 5.0f;
	TextBox(std::wstring text, int x, int y, int width = 120, int height = 24);
private:
	void InputText(std::wstring input);
	void InputBack();
	void InputDelete();
	void UpdateScroll(bool arrival = false);
public:
	std::wstring GetSelectedString();
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};