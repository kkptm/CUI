#pragma once
#include "Control.h"
#pragma comment(lib, "Imm32.lib")
class ProgressBar : public Control
{
public:
	virtual UIClass Type();
	float Boder = 1.5f;
	float PercentageValue = 0.5f;
	ProgressBar(int x, int y, int width = 120, int height = 24);
	void Update() override;
};