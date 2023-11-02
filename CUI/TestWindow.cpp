#include "TestWindow.h"
#include "Utils/Utils.h"
#include "imgs.h"
TestWindow::TestWindow() : Form(L"TestWindow", { 0,0 }, { 1240,600 })
{
    this->Init();
}
TestWindow::~TestWindow()
{
    for (int i = 0; i < 10; i++)
    {
        bmps[i]->Release();
    }
    for (int i = 0; i < 8; i++)
    {
        icos[i]->Release();
    }
}
void label1_OnMouseWheel(void* sender, MouseEventArgs e)
{
    ((Label*)sender)->Text = StringHelper::Format(L"MouseWheel Delta=[%d]", e.Delta);
    ((Label*)sender)->SingleUpdate();
}
void button1_OnMouseClick(void* sender, MouseEventArgs e)
{
    auto btn = ((Button*)sender);
    btn->Text = StringHelper::Format(L"独立Tag计数[%d]", btn->Tag++);
    btn->SingleUpdate();
}
void radiobox1_OnChecked(void* sender)
{
    ((TestWindow*)((Control*)sender)->ParentForm)->radiobox2->Checked = false;
    ((TestWindow*)((Control*)sender)->ParentForm)->radiobox2->SingleUpdate();
}
void radiobox2_OnChecked(void* sender)
{
    ((TestWindow*)((Control*)sender)->ParentForm)->radiobox1->Checked = false;
    ((TestWindow*)((Control*)sender)->ParentForm)->radiobox1->SingleUpdate();
}
void bt2_OnMouseClick(void* sender, MouseEventArgs e)
{
    TestWindow* form = ((TestWindow*)((Control*)sender)->ParentForm);
    PictureBox* picturebox1 = form->picturebox1;
    OpenFileDialog ofd;
    ofd.Filter = "图片文件(*.jpg;*.png;*.bmp;*.svg)\0*.jpg;*.png;*.bmp;*.svg\0";
    ofd.SupportMultiDottedExtensions = true;
    ofd.Title = "选择一个图片文件";
    if (ofd.ShowDialog() == DialogResult::OK)
    {
        if (picturebox1->Image)
        {
            picturebox1->Image->Release();
        }
        if (form->Image && form->Image != picturebox1->Image)
        {
            form->Image->Release();
        }
		FileInfo file(ofd.SelectedPath[0]);
        if (file.Extension() == ".svg" || file.Extension() == ".SVG")
        {
            auto bytes = File::ReadAllBytes(ofd.SelectedPath[0]);
            picturebox1->Image = form->Image = form->Render->ToBitmapFromSvg((char*)bytes.data());
			picturebox1->SingleUpdate();
		}
        else
        {
            auto bytes = File::ReadAllBytes(ofd.SelectedPath[0]);
            picturebox1->Image = form->Image = form->Render->CreateBitmap(bytes.data(), bytes.size());
			picturebox1->SingleUpdate();
		}
    }
}
void sw1_OnMouseClick(void* sender, MouseEventArgs e)
{
    Switch* sw = (Switch*)sender;
    ((TestWindow*)((Control*)sender)->ParentForm)->gridview1->Enable = sw->Checked;
}
void sw2_OnMouseClick(void* sender, MouseEventArgs e)
{
    Switch* sw = (Switch*)sender;
    ((TestWindow*)((Control*)sender)->ParentForm)->gridview1->Visable = sw->Checked;
}
void iconButton_OnMouseClick(void* sender, MouseEventArgs e)
{
    MessageBox::Show("图标按钮被单击", "模式对话框提示", MB_OK);
}
void TestWindow::Init()
{
    bmps[0] = this->Render->ToBitmapFromSvg(_0_ico);
    bmps[1] = this->Render->ToBitmapFromSvg(_1_ico);
    bmps[2] = this->Render->ToBitmapFromSvg(_2_ico);
    bmps[3] = this->Render->ToBitmapFromSvg(_3_ico);
    bmps[4] = this->Render->ToBitmapFromSvg(_4_ico);
    bmps[5] = this->Render->ToBitmapFromSvg(_5_ico);
    bmps[6] = this->Render->ToBitmapFromSvg(_6_ico);
    bmps[7] = this->Render->ToBitmapFromSvg(_7_ico);
    bmps[8] = this->Render->ToBitmapFromSvg(_8_ico);
    bmps[9] = this->Render->ToBitmapFromSvg(_9_ico);
    icos[0] = this->Render->ToBitmapFromSvg(icon0);
    icos[1] = this->Render->ToBitmapFromSvg(icon1);
    icos[2] = this->Render->ToBitmapFromSvg(icon2);
    icos[3] = this->Render->ToBitmapFromSvg(icon3);
    icos[4] = this->Render->ToBitmapFromSvg(icon4);
    this->BackColor = Colors::grey31;
    this->SizeMode = ImageSizeMode::StretchIamge;


    label1 = this->AddControl(new Label(L"Label", 10, this->HeadHeight + 10));
    label1->OnMouseWheel += label1_OnMouseWheel;
    clabel1 = this->AddControl(new CustomLabel1(L"Custom Label", 400, this->HeadHeight + 10));
    button1 = this->AddControl(new Button(L"BUTTON1", 10, this->LastChild()->Bottom + 5, 120, 24));
    button1->OnMouseClick += button1_OnMouseClick;
    textbox0 = this->AddControl(new TextBox(L"TextBox", 10, this->LastChild()->Bottom + 5, 120, 20));
    textbox1 = this->AddControl(new CustomTextBox1(L"Custom TextBox", 10, this->LastChild()->Bottom + 5, 120, 20));
    pwdbox1 = this->AddControl(new PasswordBox(L"pwd", 10, this->LastChild()->Bottom + 5, 120, 20));
    combobox1 = this->AddControl(new ComboBox(L"item1", 10, this->LastChild()->Bottom + 5, 120, 24));
    for (int i = 0; i < 10; i++)
    {
        combobox1->values.Add(StringHelper::Format(L"item%d", i));
    }
    checkbox1 = this->AddControl(new CheckBox(L"CheckBox", combobox1->Right + 5, button1->Top));
    radiobox1 = this->AddControl(new RadioBox(L"RadioBox1", combobox1->Right + 5, this->LastChild()->Bottom + 5));
    radiobox1->Checked = true;
    radiobox2 = this->AddControl(new RadioBox(L"RadioBox2", combobox1->Right + 5, this->LastChild()->Bottom + 5));
    radiobox1->OnChecked += radiobox1_OnChecked;
    radiobox2->OnChecked += radiobox2_OnChecked;
    textbox2 = this->AddControl(new RichTextBox(L"Multiline text\nMultiline text\nMultiline text\nMultiline text\nMultiline text", 260, button1->Top, 600, 60));
    textbox2->AllowMultiLine = true;
    tabControl1 = this->AddControl(new TabControl(10, combobox1->Bottom + 5, 1200, 300));
    tabControl1->BackColor = D2D1_COLOR_F{ 1,1,1,0.0 };
    tabControl1->AddPage(L"Page 1")->BackColor = D2D1_COLOR_F{ 1,1,1,0.3 };
    tabControl1->AddPage(L"Grid View")->BackColor = D2D1_COLOR_F{ 1,1,1,0.3 };
    tabControl1->AddPage(L"Icon Buttons")->BackColor = D2D1_COLOR_F{ 1,1,1,0.3 };
    tabControl1->get(0)->AddControl(new Label(L"基本容器", 10, 10))->Font = new Font(L"Arial", 12);

    bt2 = tabControl1->get(0)->AddControl(new Button(L"打开图片", 80, 10, 120, 24));
    bt2->OnMouseClick += bt2_OnMouseClick;

    panel1 = tabControl1->get(0)->AddControl(new Panel(10, 40, 400, 200));
    panel1->AddControl(new Label(L"图片框(支持拖拽)", 10, 10));
    picturebox1 = panel1->AddControl(new PictureBox(120, 10, 260, 120));
    picturebox1->Image = this->Image;
    picturebox1->OnDropFile += [](void* sender, List<std::wstring> files)
    {
        if (((Control*)sender)->Image)
        {
            ((Control*)sender)->Image->Release();
            ((Control*)sender)->Image = NULL;
        }
        FileInfo file(Convert::wstring_to_string(files[0]));
        if (file.Extension() == ".svg" || file.Extension() == ".SVG")
        {
            ((Control*)sender)->Image = ((Control*)sender)->Render->ToBitmapFromSvg((char*)File::ReadAllBytes(Convert::wstring_to_string(files[0]).c_str()).data());
            ((Control*)sender)->SingleUpdate();
        }
        else
        {
            ((Control*)sender)->Image = ((Control*)sender)->Render->CreateBitmap(files[0].c_str());
            ((Control*)sender)->SingleUpdate();
        }
    };
    panel1->AddControl(new Label(L"Progress Bar", 10, picturebox1->Bottom + 5));
    progressbar1 = panel1->AddControl(new ProgressBar(120, picturebox1->Bottom + 5, 260, 24));
    gridview1 = tabControl1->get(1)->AddControl(new GridView(10, 10, 1000, 200));
    gridview1->HeadFont = new Font(L"Arial", 24);
    gridview1->BackColor = D2D1_COLOR_F{ 0,0,0,0 };
    gridview1->Font = new Font(L"Arial", 20);
    gridview1->Colunms.Add({ L"",40,ColumnType::Image });
    gridview1->Colunms.Add({ L"check",60,ColumnType::Check });
    gridview1->Colunms.Add({ L"text",100 });
    gridview1->Colunms.Add({ L"check",60,ColumnType::Check });
    gridview1->Colunms.Add({ L"text",100 });
    for (int i = 0; i < 40; i++)
    {
        GridViewRow row;
        row.Cells = { bmps[i % 10] ,i % 2 == 0,std::to_wstring(rand()) ,i % 3 == 0 ,std::to_wstring(rand()) };
        gridview1->Rows.Add(row);
    }
    sw1 = tabControl1->get(1)->AddControl(new Switch(gridview1->Right + 5, 10));
    sw1->Checked = gridview1->Visable;
    sw1->OnMouseClick += sw1_OnMouseClick;

    sw2 = tabControl1->get(1)->AddControl(new Switch(gridview1->Right + 5, 42));
    sw2->Checked = gridview1->Visable;
    sw2->OnMouseClick += sw2_OnMouseClick;
    for (int i = 0; i < 5; i++)
    {
        Button* ingButton = tabControl1->get(2)->AddControl(new Button(L"", 10 + (44 * i), 10, 40, 40));
        ingButton->Image = icos[i];
        ingButton->SizeMode = ImageSizeMode::CenterImage;
        ingButton->BackColor = D2D1_COLOR_F{ 0,0,0,0 };
        ingButton->Boder = 2.0f;
        ingButton->OnMouseClick += iconButton_OnMouseClick;
    }
}