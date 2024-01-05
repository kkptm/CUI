# CUI
The goal of this repository is to implement a framework that supports rapid development of GUI applications(Commercially available without any copyright restrictions)

Use it like C#

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

Sample:


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
 
 QQ Group(QQ群):522222570
