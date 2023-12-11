#pragma once
#include "Control.h"
#pragma comment(lib, "Imm32.lib")
class RichTextBox : public Control
{
private:
	POINT selectedPos = {0,0};
	bool isDraggingScroll;
public:
	virtual UIClass Type();
	bool InScroll = false;
	D2D1_SIZE_F textSize = { 0,0 };
	D2D1_COLOR_F UnderMouseColor = Colors::White;
	D2D1_COLOR_F SelectedBackColor = Colors::Blue;
	D2D1_COLOR_F SelectedForeColor = Colors::White;
	D2D1_COLOR_F FocusedColor = Colors::White;
	D2D1_COLOR_F ScrollBackColor = Colors::LightGray;
	D2D1_COLOR_F ScrollForeColor = Colors::DimGrey;
	bool AllowMultiLine = false;
	int SelectionStart = 0;
	int SelectionEnd = 0;
	float Boder = 1.5f;
	float OffsetY = 0.0f;
	float TextMargin = 5.0f;
	RichTextBox(std::wstring text, int x, int y, int width = 120, int height = 24);
private:
	void DrawScroll();
	void UpdateScrollDrag(float posY);
	void SetScrollByPos(float yof);
	void InputText(std::wstring input);
	void InputBack();
	void InputDelete();
	void UpdateScroll(bool arrival = false);
public:
	void AppendText(std::wstring str);
	void AppendLine(std::wstring str);
	std::wstring GetSelectedString();
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};