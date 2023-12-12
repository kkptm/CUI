#pragma once
#include "Form.h"
class ProcessSelectWindow : public Form
{
	TextBox* _searchBox = NULL;
	GridView* _table = NULL;
	Button* _selectButton = NULL;
public:
	HANDLE SelectedProcessId = (HANDLE)-1;
	static void _searchBox_OnTextChanged(class Control* sender, const std::wstring oldtext, const std::wstring text);
	static void _selectButton_OnMouseClick(class Control* sender, MouseEventArgs e);
	ProcessSelectWindow();
	~ProcessSelectWindow();

};

