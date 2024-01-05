# CUI
The goal of this repository is to implement a framework that supports rapid development of GUI applications(Commercially available without any copyright restrictions)

Use it like C#,Use Direct2D hardware acceleration,Powerful drawing function library,Basic libraries (strings, files, streams, registry, clipboard, conversions, events, HTTP/s, processes, threads, TCP/UDP, containers like C#)

Form
Button
CheckBox
ComboBox
GridView
Label
Panel
PasswordBox
PictureBox
ProgressBar
RadioBox
RichTextBox
RoundTextBox
Switch
TabControl
TextBox
TreeView
NotifyIcon
Taskbar

Simplest example:


	class TestWindow : public Form
	{
	private:
	    void button1_OnMouseClick(void* sender, MouseEventArgs e)
	    {
	        Button* btn = (Button*)sender;
	        btn->Text = std::to_string(btn->Flag++);
	        TestWindow* window = (TestWindow*)btn->btn->ParentForm;
	        MessageBoxA(window->Handle,"button1_OnMouseClick","",MB_OK);
	    }
	public:
	    Button* button1;
	    TestWindow() : Form(L"TestWindow", { 0,0 }, { 1240,600 })
	    {
	        button1 = (Button*)this->AddControl(new Button(L"BUTTON1", 10, 10, 120, 24));
	        button1->OnMouseClick += button1_OnMouseClick;
	    }
	};
	void main()
	{
	    TestWindow* mainForm = new TestWindow();
	    mainForm->Show();
	    while(1)
	        Form::DoEvent();
	}
 
 A simple process selector:
 
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
	 void ProcessSelectWindow::_searchBox_OnTextChanged(class Control* sender, const std::wstring oldtext, const std::wstring text)
	{
		TextBox* _tx = (TextBox*)sender;
		ProcessSelectWindow* window = (ProcessSelectWindow*)_tx->ParentForm;
		window->_table->Clear(); 
		window->_table->SelectedRowIndex = -1;
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
	void ProcessSelectWindow::_selectButton_OnMouseClick(class Control* sender, MouseEventArgs e)
	{
		Button* _btn = (Button*)sender;
		ProcessSelectWindow* window = (ProcessSelectWindow*)_btn->ParentForm;
		if (window->_table->SelectedRowIndex >= 0 && window->_table->SelectedRowIndex < window->_table->Rows.Count)
		{
			window->Close();
			window->SelectedProcessId = (HANDLE)window->_table->SelectedRow().Cells[1].Tag;
		}
	}
	ProcessSelectWindow::ProcessSelectWindow() : Form(L"Select a process", { 0,0 }, { 400,540 })
	{
		this->SelectedProcessId = (HANDLE)-1;
	
		_searchBox = new TextBox(L"", 10, this->HeadHeight + 10, 370, 30);
		_searchBox->BackColor = Colors::GhostWhite;
		_table = new GridView(10, this->HeadHeight + 50, 370, 400);
		_selectButton = new Button(L"确定", 10, this->HeadHeight + 460, 370, 30);
	
	
		_table->Colunms.push_back(GridViewColunm{ L"进程名", 200 , ColumnType::Text, false });
		_table->Colunms.push_back(GridViewColunm{ L"PID", 60 , ColumnType::Text, false });
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
		this->MinBox = false;
		this->MaxBox = false;
	}
	
	ProcessSelectWindow::~ProcessSelectWindow()
	{
		_table->Rows.resize(0);
		delete _searchBox;
		delete _table;
		delete _selectButton;
	}
 
 QQ Group(QQ群):522222570
