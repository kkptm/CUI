#pragma once
#include <windows.h>
#include <string>
#include "../Utils/Event.h"
class NotifyIcon
{
private:
	NOTIFYICONDATAA NotifyIconData;
	int iconID;
public:
	HWND hWnd;
	static class NotifyIcon* Instance;
	NotifyIcon();
	~NotifyIcon();
	void InitNotifyIcon(HWND hWnd, int iconID);
	void SetIcon(HICON hIcon);
	void ShowNotifyIcon();
	void HideNotifyIcon();
	void SetToolTip(const char* text);
	void ShowBalloonTip(const char* title, const char* text, int timeout = 5000, int type = NIIF_INFO);

	typedef Event<void(*)(class NotifyIcon*, MouseEventArgs)> NotifyIconMouseDownEvent;
	NotifyIconMouseDownEvent OnNotifyIconMouseDown;
};

