#include "Clipboard.h"
#include <ShlObj.h>
#include <oleidl.h>
#include <comdef.h>
std::string Clipboard::GetText()
{
	if (!OpenClipboard(nullptr))
		return "";
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return "";
	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return "";
	std::string text(pszText);
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}
void Clipboard::SetText(std::string str)
{
	if (!OpenClipboard(nullptr))
		return;
	EmptyClipboard();
	HGLOBAL hClipboardData;
	hClipboardData = GlobalAlloc(GMEM_DDESHARE, str.size() + 1);
	char* pchData;
	pchData = static_cast<char*>(GlobalLock(hClipboardData));
	strcpy_s(pchData, str.size() + 1, LPCSTR(str.c_str()));
	GlobalUnlock(hClipboardData);
	SetClipboardData(CF_TEXT, hClipboardData);
	CloseClipboard();
}
void Clipboard::Clear()
{
	if (!OpenClipboard(nullptr))
		return;
	EmptyClipboard();
	CloseClipboard();
}
void Clipboard::SetFile(std::string files)
{
	UINT uDropEffect;
	HGLOBAL hGblEffect;
	LPDWORD lpdDropEffect;
	DROPFILES stDrop;

	HGLOBAL hGblFiles;
	LPSTR lpData;

	uDropEffect = RegisterClipboardFormatA("Preferred DropEffect");
	hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD));
	lpdDropEffect = (LPDWORD)GlobalLock(hGblEffect);
	*lpdDropEffect = DROPEFFECT_COPY;
	GlobalUnlock(hGblEffect);

	stDrop.pFiles = sizeof(DROPFILES);
	stDrop.pt.x = 0;
	stDrop.pt.y = 0;
	stDrop.fNC = FALSE;
	stDrop.fWide = FALSE;

	hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, \
		sizeof(DROPFILES) + files.size() + 2);
	lpData = (LPSTR)GlobalLock(hGblFiles);
	memcpy(lpData, &stDrop, sizeof(DROPFILES));
	memcpy(lpData + sizeof(DROPFILES), files.c_str(), files.size() + 1);
	GlobalUnlock(hGblFiles);

	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_HDROP, hGblFiles);
	SetClipboardData(uDropEffect, hGblEffect);
	CloseClipboard();
}
void Clipboard::SetFiles(std::vector<std::string> files)
{
    UINT uDropEffect;
	HGLOBAL hGblEffect;
	LPDWORD lpdDropEffect;
	DROPFILES stDrop;
	HGLOBAL hGblFiles;
	LPSTR lpData;
	uDropEffect = RegisterClipboardFormatA("Preferred DropEffect");
	hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD));
	lpdDropEffect = (LPDWORD)GlobalLock(hGblEffect);
	*lpdDropEffect = DROPEFFECT_COPY;
	GlobalUnlock(hGblEffect);
	stDrop.pFiles = sizeof(DROPFILES);
	stDrop.pt.x = 0;
	stDrop.pt.y = 0;
	stDrop.fNC = FALSE;
	stDrop.fWide = FALSE;
	std::string files_str;
	for (auto &file : files)
	{
		files_str += file;
		files_str += '\0';
	}
	hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, \
		sizeof(DROPFILES) + files_str.size() + 2);
	lpData = (LPSTR)GlobalLock(hGblFiles);
	memcpy(lpData, &stDrop, sizeof(DROPFILES));
	memcpy(lpData + sizeof(DROPFILES), files_str.c_str(), files_str.size() + 1);
	GlobalUnlock(hGblFiles);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_HDROP, hGblFiles);
	SetClipboardData(uDropEffect, hGblEffect);
	CloseClipboard();
}
std::string Clipboard::GetFile()
{
	std::string path;
	if (::OpenClipboard(NULL))
	{
		HDROP hDrop = HDROP(::GetClipboardData(CF_HDROP));
		if (hDrop != NULL)
		{
			WCHAR szFilePathName[MAX_PATH + 1] = { 0 };
			UINT nNumOfFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			if (nNumOfFiles == 1)
			{
				memset(szFilePathName, 0, MAX_PATH + 1);
				DragQueryFile(hDrop, 0, szFilePathName, MAX_PATH);
				_bstr_t path(szFilePathName);
				path = (LPCSTR)path;
			}
		}
		CloseClipboard();
	}

	return path;
}
std::vector<std::string> Clipboard::GetFiles()
{
	std::vector<std::string> path_list;
	if (::OpenClipboard(NULL))
	{
		HDROP hDrop = HDROP(::GetClipboardData(CF_HDROP));
		if (hDrop != NULL)
		{
			WCHAR szFilePathName[MAX_PATH + 1] = { 0 };
			UINT nNumOfFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
			{
				memset(szFilePathName, 0, MAX_PATH + 1);
				DragQueryFile(hDrop, nIndex, szFilePathName, MAX_PATH);
				_bstr_t path(szFilePathName);
				std::string ss = (LPCSTR)path;
				path_list.push_back(ss);
			}
		}
		CloseClipboard();
	}

	return path_list;
}
void Clipboard::SetImage(HBITMAP bmp)
{
	if (!OpenClipboard(nullptr))
		return;
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bmp);
	CloseClipboard();
}
HBITMAP Clipboard::GetImage()
{
	if (!OpenClipboard(nullptr))
		return nullptr;
	HANDLE hData = GetClipboardData(CF_BITMAP);
	if (hData == nullptr)
		return nullptr;
	CloseClipboard();
	return (HBITMAP)hData;
}
UINT Clipboard::GetFormat()
{
	if (!OpenClipboard(nullptr))
		return 0;
	auto format = GetPriorityClipboardFormat(nullptr, 0);
	CloseClipboard();
	return format;
}