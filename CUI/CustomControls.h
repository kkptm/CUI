#pragma once
#include "GUI/TextBox.h"
#include "GUI/Label.h"
class CustomTextBox1 : public TextBox
{
public:
	std::vector<D2D1_GRADIENT_STOP> Stops;
	CustomTextBox1(std::wstring text, int x, int y, int width = 120, int height = 24);
	void Update() override;
};
class CustomLabel1 : public Label
{
public:
	std::vector<D2D1_GRADIENT_STOP> Stops;
	CustomLabel1(std::wstring text, int x, int y);
	void Update() override;
};