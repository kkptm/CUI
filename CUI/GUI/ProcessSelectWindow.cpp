#include "ProcessSelectWindow.h"

void ProcessSelectWindow::_searchBox_OnTextChanged(void* sender, const std::wstring oldtext, const std::wstring text)
{
	TextBox* _tx = (TextBox*)sender;
	ProcessSelectWindow* window = (ProcessSelectWindow*)_tx->ParentForm;
	window->_table->Clear();
	std::vector<Process> processes = Process::GetProcesses();
	for (auto p : processes)
	{
		if (StringHelper::ToLower(p.ProcessName).find(StringHelper::ToLower(text)) != std::wstring::npos)
		{
			GridViewRow row;
			CellValue pid = std::to_wstring(p.Id);
			pid.Tag = p.Id;
			row.Cells = { p.ProcessName,pid };
			window->_table->Rows.Add(row);
		}
	}

}
void ProcessSelectWindow::_selectButton_OnMouseClick(void* sender, MouseEventArgs e)
{
	Button* _btn = (Button*)sender;
	ProcessSelectWindow* window = (ProcessSelectWindow*)_btn->ParentForm;
	if (window->_table->SelectedRowIndex >= 0)
	{
		auto pid = (HANDLE)window->_table->SelectedRow().Cells[1].Tag;
		window->Close();
		window->SelectedProcessId = (HANDLE)pid;
	}
}
ProcessSelectWindow::ProcessSelectWindow() : Form(L"选择进程", { 0,0 }, { 400,540 })
{
	this->SelectedProcessId = (HANDLE)-1;

	_searchBox = new TextBox(L"", 10, 10, 370, 30);
	_table = new GridView(10, 50, 370, 400);
	_selectButton = new Button(L"确定", 10, 460, 370, 30);


	_table->Colunms.push_back(GridViewColunm{ L"进程名", 200 , ColumnType::Text });
	_table->Colunms.push_back(GridViewColunm{ L"PID", 60 , ColumnType::Text });
	std::vector<Process> processes = Process::GetProcesses();
	for (auto p : processes)
	{
		GridViewRow row;
		CellValue pid = std::to_wstring(p.Id);
		pid.Tag = p.Id;
		row.Cells = { p.ProcessName,pid };
		_table->Rows.Add(row);
	}

	_searchBox->OnTextChanged += _searchBox_OnTextChanged;
	_selectButton->OnMouseClick += _selectButton_OnMouseClick;

	this->AddControl(_searchBox);
	this->AddControl(_table);
	this->AddControl(_selectButton);
}

ProcessSelectWindow::~ProcessSelectWindow()
{
	_table->Rows.resize(0);
	delete _searchBox;
	delete _table;
	delete _selectButton;
}