#pragma once
#include "Control.h"
#pragma comment(lib, "Imm32.lib")
class Label : public Control
{
public:
	float last_width = 0.0f;
	virtual UIClass Type();
	Label(std::wstring text, int x, int y);
	SIZE ActualSize() override;
	void Update() override;
};