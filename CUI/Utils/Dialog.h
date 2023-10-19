#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <commdlg.h>
#include <Shlobj.h>
#include "defines.h"
#undef MessageBox

HWND GetCurrentActiveWindow();
enum class DialogResult
{
	None,
	OK,
	Cancel,
	Abort,
	Retry,
	Ignore,
	Yes,
	No
};
class OpenFileDialog
{
public:
	std::vector<std::string> SelectedPath;
	std::string InitialDirectory;
	const char* Filter;
	int FilterIndex;
	bool Multiselect;
	bool SupportMultiDottedExtensions;
	bool DereferenceLinks;
	bool ValidateNames;
	std::string Title;
	OpenFileDialog();
	DialogResult ShowDialog();
}; 
class SaveFileDialog
{
public:
	std::string SelectedPath;
	std::string InitialDirectory;
	const char* Filter;
	int FilterIndex;
	bool SupportMultiDottedExtensions;
	bool DereferenceLinks;
	bool ValidateNames;
	std::string Title;
	SaveFileDialog();
	DialogResult ShowDialog();
};
class FolderBrowserDialog
{
public:
	std::string SelectedPath;
	std::string Description;
	bool ShowNewFolderButton;
	std::string RootFolder;
	FolderBrowserDialog();
	DialogResult ShowDialog();
};
class ColorDialog
{
public:
COLORREF Color;
ColorDialog();
	DialogResult ShowDialog();
};
class FontDialog
{
public:
	std::string FontName;
	int FontSize;
	COLORREF Color;
	bool Bold;
	bool Italic;
	bool Underline;
	bool Strikeout;
	FontDialog();
	DialogResult ShowDialog();
};
class MessageBox
{
public:
	static DialogResult Show(const std::string& text, const std::string& caption = "", UINT type = MB_OK);
};
