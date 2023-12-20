#pragma once
#include <iostream>
#include "GUI/Form.h"
#include "CustomControls.h"
class TestWindow : public Form
{
public:
    ID2D1Bitmap* bmps[10];
    ID2D1Bitmap* icos[8];
    Label* label1;
    CustomLabel1* clabel1;
    Button* button1;
    TextBox* textbox0;
    CustomTextBox1* textbox1;
    RoundTextBox* textbox3;
    PasswordBox* pwdbox1;
    RichTextBox* textbox2;
    ComboBox* combobox1;
    CheckBox* checkbox1;
    RadioBox* radiobox1;
    RadioBox* radiobox2;
    TabControl* tabControl1;
    Panel* panel1;
    PictureBox* picturebox1;
    ProgressBar* progressbar1;
    GridView* gridview1;
    Button* bt2;
    Switch* sw1;
    Switch* sw2;
    TestWindow();
    ~TestWindow();
    void Init();
};
