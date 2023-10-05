#pragma once
#include "Control.h"
#pragma comment(lib, "Imm32.lib")
class Panel : public Control
{
public:
	virtual UIClass Type();
	float Boder = 1.5f;
	Panel();
	Panel(int x, int y, int width = 120, int height = 24);
	void Update() override;
	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, int xof, int yof) override;
};