#include "Dialog.h"

HWND GetCurrentActiveWindow()
{
	HWND hwnd = GetForegroundWindow();
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == GetCurrentProcessId())
	{
		return hwnd;
	}
	else
	{
		return NULL;
	}
}

OpenFileDialog::OpenFileDialog() :FilterIndex(0), Multiselect(false)
{
}
DialogResult OpenFileDialog::ShowDialog()
{
	OPENFILENAMEA file = { 0 };
	char szFile[260] = { 0 };
	file.lStructSize = sizeof(file);
	file.hwndOwner = GetCurrentActiveWindow();
	file.lpstrFile = szFile;
	file.nMaxFile = sizeof(szFile);
	file.lpstrFilter = Filter;
	file.nFilterIndex = FilterIndex;
	file.lpstrFileTitle = NULL;
	if(InitialDirectory.size() > 0) file.lpstrInitialDir = InitialDirectory.c_str();
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	if (this->Multiselect)
	{
		file.Flags |= OFN_ALLOWMULTISELECT;
	}
	if (this->SupportMultiDottedExtensions)
	{
		file.Flags |= OFN_EXPLORER | OFN_ENABLESIZING;
	}
	file.lpstrTitle = Title.c_str();
	file.lpstrDefExt = NULL;
	if (GetOpenFileNameA(&file))
	{
		if (this->Multiselect)
		{
			char* p = szFile;
			while (*p)
			{
				std::string str = std::string(p);
				if (str.find("\\") == std::string::npos)
				{
					this->SelectedPath.push_back(str);
					break;
				}
				else
				{
					this->SelectedPath.push_back(str);
					p += str.length() + 1;
				}
			}
		}
		else
		{
			this->SelectedPath.push_back(szFile);
		}
		return DialogResult::OK;
	}
	else
	{
		return DialogResult::Cancel;
	}
}
SaveFileDialog::SaveFileDialog() :FilterIndex(0)
{
}
DialogResult SaveFileDialog::ShowDialog()
{
	OPENFILENAMEA file = { 0 };
	char szFile[260] = { 0 };
	file.lStructSize = sizeof(file);
	file.hwndOwner = GetCurrentActiveWindow();
	file.lpstrFile = szFile;
	file.nMaxFile = sizeof(szFile);
	file.lpstrFilter = Filter;
	file.nFilterIndex = FilterIndex;
	file.lpstrFileTitle = NULL;
	file.nMaxFileTitle = 0;
	file.lpstrInitialDir = InitialDirectory.c_str();
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	if (this->SupportMultiDottedExtensions)
	{
		file.Flags |= OFN_EXPLORER | OFN_ENABLESIZING | OFN_ALLOWMULTISELECT | OFN_ENABLEHOOK;
	}
	file.lpstrTitle = Title.c_str();
	file.lpstrDefExt = NULL;
	if (GetSaveFileNameA(&file))
	{
		this->SelectedPath = szFile;
		return DialogResult::OK;
	}
	else
	{
		return DialogResult::Cancel;
	}
}
FolderBrowserDialog::FolderBrowserDialog() :ShowNewFolderButton(true)
{
}
DialogResult FolderBrowserDialog::ShowDialog()
{
	BROWSEINFOA bi = { 0 };
	bi.hwndOwner = GetCurrentActiveWindow();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = Description.c_str();
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	if (ShowNewFolderButton)
	{
		bi.ulFlags |= BIF_NEWDIALOGSTYLE;
	}
	LPITEMIDLIST idl = SHBrowseForFolderA(&bi);
	if (idl == NULL)
	{
		return DialogResult::Cancel;
	}
	else
	{
		char path[MAX_PATH] = { 0 };
		SHGetPathFromIDListA(idl, path);
		this->SelectedPath = path;
		return DialogResult::OK;
	}
}
ColorDialog::ColorDialog() :Color(RGB(0, 0, 0))
{
}
DialogResult ColorDialog::ShowDialog()
{
	CHOOSECOLORA cc = { 0 };
	static COLORREF acrCustClr[16];
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = GetCurrentActiveWindow();
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = Color;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColorA(&cc))
	{
		this->Color = cc.rgbResult;
		return DialogResult::OK;
	}
	else
	{
		return DialogResult::Cancel;
	}
}
FontDialog::FontDialog() :FontSize(9), Color(RGB(0, 0, 0)), Bold(false), Italic(false), Underline(false), Strikeout(false)
{
}
DialogResult FontDialog::ShowDialog()
{
	CHOOSEFONTA cf = { 0 };
	static LOGFONTA lf = { 0 };
	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = GetCurrentActiveWindow();
	cf.lpLogFont = &lf;
	cf.rgbColors = Color;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
	if (ChooseFontA(&cf))
	{
		int fontSizeInPixels = abs(lf.lfHeight);
		int dpi = GetDeviceCaps(GetDC(cf.hwndOwner), LOGPIXELSY);
		int fontSize = MulDiv(fontSizeInPixels, 72, dpi);

		this->FontName = lf.lfFaceName;
		this->FontSize = fontSize;
		this->Color = cf.rgbColors;
		this->Bold = lf.lfWeight == FW_BOLD;
		this->Italic = lf.lfItalic;
		this->Underline = lf.lfUnderline;
		this->Strikeout = lf.lfStrikeOut;
		return DialogResult::OK;
	}
	else
	{
		return DialogResult::Cancel;
	}
}
DialogResult MessageBox::Show(const std::string& text, const std::string& caption, UINT type)
{
	return (DialogResult)MessageBoxA(GetCurrentActiveWindow(), text.c_str(), caption.c_str(), type);
}
