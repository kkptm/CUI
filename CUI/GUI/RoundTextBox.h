#pragma once
#include "Control.h"
#include "TextBox.h"
#pragma comment(lib, "Imm32.lib")
class RoundTextBox : public TextBox
{
public:
	RoundTextBox(std::wstring text, int x, int y, int width = 120, int height = 24);
public:
	void Update() override;
};