#pragma once
#include "defines.h"
#include <Psapi.h>
#include <string>
#include <vector>
class ProcessOperator
{
public:
	HANDLE Handle = NULL;
	DWORD PID = NULL;
	ProcessOperator(DWORD pid);
	BOOL Open();
	BOOL Close();
	BOOL Read(ULONG64 addr, const PVOID buffer, SIZE_T size);
	BOOL Write(ULONG64 addr, const PVOID buffer, SIZE_T size);
	long long VirtualMemorySize();
	std::wstring CommandLine();
	//仅限于64位进程
	ULONG64 InjectDll(BYTE* buffer, SIZE_T size);
	ULONG64 AllocateMemory(SIZE_T size, DWORD protect = PAGE_READWRITE, DWORD type = MEM_COMMIT, ULONG64 baseAddr = NULL);
	ULONG64 AllocateString(std::string s);
	ULONG64 AllocateWString(std::wstring s);
	BOOL FreeMemory(ULONG64 addr, SIZE_T size = NULL, DWORD freeType = MEM_DECOMMIT);
	//仅限于64位进程
	ULONG64 CallRemote(ULONG64 func, std::vector<ULONG64> args = {});
	//仅限于64位进程
	ULONG64 CallRemote(ULONG64 func,
		ULONG64 a1 = NULL, ULONG64 a2 = NULL, ULONG64 a3 = NULL, ULONG64 a4 = NULL,
		ULONG64 a5 = NULL, ULONG64 a6 = NULL, ULONG64 a7 = NULL, ULONG64 a8 = NULL
	);
	ULONG64 FindPattern(const char* szModule, const char* sPattern, int offset = 0);
	ULONG64 FindPattern(ULONG64 _begin, const char* sPattern, int search_size, int offset = 0);

	template<typename T>
	T Allocate(T val)
	{
		ULONG64 addr = this->AllocateMemory(sizeof(T));
		if (addr)
		{
			this->Write(addr, val);
			return addr;
		}
		return NULL;
	}
	template<typename T>
	T Read(ULONG64 addr)
	{
		T val;
		ReadProcessMemory(this->Handle, PVOID(addr), &val, sizeof(T), NULL);
		return val;
	}
	template<typename T>
	void Write(ULONG64 addr, T val)
	{
		WriteProcessMemory(this->Handle, PVOID(addr), &val, sizeof(T), NULL);
	}
};

