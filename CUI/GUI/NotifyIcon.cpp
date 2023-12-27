#include "NotifyIcon.h"
NotifyIcon* NotifyIcon::Instance = NULL;
NotifyIcon::NotifyIcon()
{
	ZeroMemory(&NotifyIconData, sizeof(NOTIFYICONDATAA));
	NotifyIconData.cbSize = sizeof(NOTIFYICONDATAA);
	NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	NotifyIconData.uCallbackMessage = WM_USER + 1;
	NotifyIconData.hIcon = NULL;
	NotifyIconData.hWnd = NULL;
	NotifyIconData.uID = 0;
	NotifyIconData.uVersion = NOTIFYICON_VERSION;
}
NotifyIcon::~NotifyIcon()
{
	HideNotifyIcon();
}
void NotifyIcon::InitNotifyIcon(HWND hWnd, int iconID)
{
	this->hWnd = hWnd;
	this->iconID = iconID;
	NotifyIconData.hWnd = hWnd;
	NotifyIconData.uID = iconID;
}
void NotifyIcon::SetIcon(HICON hIcon)
{
	NotifyIconData.hIcon = hIcon;
}
void NotifyIcon::ShowNotifyIcon()
{
	Shell_NotifyIconA(NIM_ADD, &NotifyIconData);
	Instance = this;
}
void NotifyIcon::HideNotifyIcon()
{
	Shell_NotifyIconA(NIM_DELETE, &NotifyIconData);
	if(Instance == this)
		Instance = NULL;
}
void NotifyIcon::SetToolTip(const char* text)
{
	strncpy_s(NotifyIconData.szTip, text, sizeof(NotifyIconData.szTip));
}
void NotifyIcon::ShowBalloonTip(const char* title, const char* text, int timeout, int type)
{
	NotifyIconData.uFlags |= NIF_INFO;
	NotifyIconData.dwInfoFlags = type;
	strncpy_s(NotifyIconData.szInfoTitle, title, sizeof(NotifyIconData.szInfoTitle));
	strncpy_s(NotifyIconData.szInfo, text, sizeof(NotifyIconData.szInfo));
	NotifyIconData.uTimeout = timeout;
	Shell_NotifyIconA(NIM_MODIFY, &NotifyIconData);
}

