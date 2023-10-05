#pragma once
#include "GUI/TextBox.h"
#include "GUI/Label.h"
class CustomTextBox1 : public TextBox
{
public:
	CustomTextBox1(std::wstring text, int x, int y, int width = 120, int height = 24);
public:
	void Update() override;
};
class CustomLabel1 : public Label
{
public:
	CustomLabel1(std::wstring text, int x, int y);
	void Update() override;
};