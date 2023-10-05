#pragma once
#include <Windows.h>
#include "../Utils/Utils.h"
class Application
{
public:
	static Dictionary<HWND,class Form*> Forms;
	static std::string ExecutablePath();
	static std::string StartupPath();
	static std::string ApplicationName();
	static std::string LocalUserAppDataPath();
	static std::string UserAppDataPath();
	static RegistryKey UserAppDataRegistry();

};