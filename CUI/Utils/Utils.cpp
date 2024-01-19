#include "Utils.h"
#include <locale>
#include <Psapi.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <iostream>
#include <stdexcept>
#include <TlHelp32.h>

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
DWORD NtBuildVersion()
{
	static DWORD res = 0;
	if (!res) {
		typedef LONG(NTAPI* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
		fnRtlGetVersion pRtlGetVersion = NULL;
		while (pRtlGetVersion == 0)
		{
			HMODULE ntdll = GetModuleHandleA("ntdll.dll");
			if (ntdll) {
				pRtlGetVersion = (fnRtlGetVersion)GetProcAddress(ntdll, "RtlGetVersion");
			}
		}
		RTL_OSVERSIONINFOW osversion{};
		pRtlGetVersion(&osversion);
		res = osversion.dwBuildNumber;
	}
	return res;
}
char* ToHex(const char* data, size_t len)
{
	const char* key = "0123456789ABCDEF";
	char* result = new char[len * 2];
	for (int i = 0; i < len; i++)
	{
		result[i * 2] = key[(BYTE)(data[i] >> 4)];
		result[(i * 2) + 1] = key[(BYTE)((BYTE)(data[i] << 4) >> 4)];
	}
	return result;
}
char* ToHex(const BYTE* data, size_t len)
{
	const char* key = "0123456789ABCDEF";
	char* result = new char[(len * 2) + 1];
	for (int i = 0; i < len; i++)
	{
		result[i * 2] = key[(BYTE)(data[i] >> 4)];
		result[(i * 2) + 1] = key[(BYTE)((BYTE)(data[i] << 4) >> 4)];
	}
	result[len * 2] = '\0';
	return result;
}
std::vector<BYTE> ParseHex(const char* hex)
{
	std::vector<BYTE> result = std::vector<BYTE>();
	int len = strlen(hex);
	BYTE* buffer = new BYTE[len];
	bool ish = true;
	BYTE val = 0;
	for (int i = 0; i < len; i++)
	{
		bool isval = false;
		BYTE v = 0;
		if (hex[i] >= '0' && hex[i] <= '9')
		{
			v = (hex[i] - '0');
			isval = true;
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F')
		{
			v = 10 + (hex[i] - 'A');
			isval = true;
		}
		else if (hex[i] >= 'a' && hex[i] <= 'f')
		{
			v = 10 + (hex[i] - 'a');
			isval = true;
		}
		if (isval)
		{
			if (ish)
			{
				val = (v << 4) | val;
			}
			else
			{
				val = v | val;
				result.push_back(val);
				val = 0;
			}
			ish = !ish;
		}
	}
	return result;
}
std::vector<__int16> ParserPattern(const char* text)
{
#define __IS_HEX(c) (c>='0'&&c<='9') ||  (c>='a'&&c<='f') ||  (c>='A'&&c<='F')
#define HextoValue(c) ((c >= '0' && c <= '9') ? c - 48 :(c >= 'a' && c <= 'f') ? c - 87 :(c >= 'A' && c <= 'F') ? c - 55 :0)
	std::vector<__int16> result = std::vector<__int16>();
	int len = strlen(text), clen = 0;
	unsigned char tmp = 0;
	for (int i = 0; i < len; i++) {
		if (__IS_HEX(text[i])) {
			if (clen == 0) {
				tmp = (unsigned char)(HextoValue(text[i]) << 4);
				clen += 1;
			}
			else if (clen == 1) {
				tmp |= HextoValue(text[i]);
				result.push_back(tmp);
				clen = 0;
			}
		}
		else {
			if (text[i] == '?') {
				result.push_back(-1);
				if (text[i + 1] == '?') i += 1;
			}
			clen = 0;
		}
	}
	return result;
}
void* FindPattern(const char* szModule, const char* sPattern, int offset)
{
	std::vector<__int16> pattern = ParserPattern(sPattern);
	if (pattern.size() == 0) return NULL;
	MODULEINFO mi{ };
	if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &mi, sizeof(mi)))
	{
		unsigned char* begin = (unsigned char*)mi.lpBaseOfDll;
		DWORD size = mi.SizeOfImage;
		for (unsigned char* p = begin + offset; p < (begin + size) - (pattern.size() + 1); p++)
		{
			for (int o = 0; o < pattern.size(); o++)
			{
				if (p[o] != pattern[o] && pattern[o] != -1)
				{
					goto nxt;
				}
			}
			return p;
		nxt:;
		}
	}
	return NULL;
}
std::vector<void*> FindAllPattern(const char* szModule, const char* sPattern, int offset)
{
	std::vector<void*> result = std::vector<void*>();
	std::vector<__int16> pattern = ParserPattern(sPattern);
	if (pattern.size() == 0) return result;
	MODULEINFO mi{ };
	HMODULE m = NULL;
	if (szModule)
		m = LoadLibraryA(szModule);
	else
		m = GetModuleHandleA(NULL);
	if (!m)
	{
		printf("GetModule Infomation Failed!\n");
		return result;
	}
	if (GetModuleInformation(GetCurrentProcess(), m, &mi, sizeof(mi)))
	{
		unsigned char* begin = (unsigned char*)mi.lpBaseOfDll;
		DWORD size = mi.SizeOfImage;
		for (unsigned char* p = begin + offset; p < (begin + size) - (pattern.size() + 1); p++)
		{
			for (int o = 0; o < pattern.size(); o++)
			{
				if (p[o] != pattern[o] && pattern[o] != -1)
				{
					goto nxt;
				}
			}
			result.push_back(p);
		nxt:;
		}
	}
	else
	{
		printf("GetModule Infomation Failed!\n");
	}
	return result;
}
void* FindPattern(void* _begin, const char* sPattern, int search_size, int offset)
{
	std::vector<__int16> pattern = ParserPattern(sPattern);
	if (pattern.size() == 0) return NULL;
	unsigned char* begin = (unsigned char*)_begin;
	for (unsigned char* p = begin + offset; p < (begin + search_size) - (pattern.size() + 1); p++)
	{
		for (int o = 0; o < pattern.size(); o++)
		{
			if (p[o] != pattern[o] && pattern[o] != -1)
			{
				goto nxt;
			}
		}
		return p;
	nxt:;
	}
	return NULL;
}
std::vector<void*> FindAllPattern(void* _begin, const char* sPattern, int search_size, int offset)
{
	std::vector<void*> result = std::vector<void*>();
	std::vector<__int16> pattern = ParserPattern(sPattern);
	if (pattern.size() == 0) return result;
	unsigned char* begin = (unsigned char*)_begin;
	for (unsigned char* p = begin + offset; p < (begin + search_size) - (pattern.size() + 1); p++)
	{
		for (int o = 0; o < pattern.size(); o++)
		{
			if (p[o] != pattern[o] && pattern[o] != -1)
			{
				goto nxt;
			}
		}
		result.push_back(p);
	nxt:;
	}
	return result;
}
void PrintHex(void* ptr, int count, int splitLine)
{
	const char keys[] = "0123456789ABCDEF";
	BYTE* tmp = (BYTE*)ptr;
	for (BYTE* b = tmp; b < tmp + count; b += splitLine)
	{
		for (int j = 0; j < splitLine && b + j < tmp + count; j++)
		{
			printf("%c%c ", keys[b[j] / 0x10], keys[b[j] % 0x10]);
		}
		printf("\n");
	}
}
void PrintHex(void* ptr, int count)
{
	const char keys[] = "0123456789ABCDEF";
	BYTE* tmp = (BYTE*)ptr;
	for (BYTE* b = tmp; b < tmp + count; b++)
	{
		printf("%c%c ", keys[*b / 0x10], keys[*b % 0x10]);
	}
}
__int64 GetTick()
{
	return *(__int64*)0x7FFE0348;
}
void MakePermute(std::vector<int> nums, std::vector<std::vector<int>>& result, int start) {
	if (start == nums.size() - 1) {
		result.push_back(nums);
		return;
	}

	for (int i = start; i < nums.size(); i++) {
		std::swap(nums[start], nums[i]);
		MakePermute(nums, result, start + 1);
		std::swap(nums[start], nums[i]);
	}
}

std::string GetLastErrorMessage()
{
	DWORD err = GetLastError();
	LPVOID errorMsgBuffer;
	DWORD size = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		err,
		0,
		(LPWSTR)&errorMsgBuffer,
		0,
		nullptr
	);
	std::string errorMessage;
	if (size != 0)
	{
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)errorMsgBuffer, -1, nullptr, 0, nullptr, nullptr);
		if (bufferSize > 0)
		{
			std::string narrowErrorMsg(bufferSize, 0);
			WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)errorMsgBuffer, -1, &narrowErrorMsg[0], bufferSize, nullptr, nullptr);
			errorMessage = narrowErrorMsg;
		}
		LocalFree(errorMsgBuffer);
	}
	else
	{
		errorMessage = "Î´ÖªµÄ´íÎó";
	}
	return errorMessage;
}

PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID Base)
{
	static PIMAGE_NT_HEADERS(*_RtlImageNtHeader)(PVOID Base) = NULL;
	if (_RtlImageNtHeader == NULL)
	{
		HMODULE NtBase = GetModuleHandleA("ntdll.dll");
		_RtlImageNtHeader = (decltype(_RtlImageNtHeader))GetProcAddress(NtBase, "RtlImageNtHeader");
	}
	return _RtlImageNtHeader(Base);
}

SIZE_T GetSectionSize(_In_ PVOID DllBase)
{
	PIMAGE_NT_HEADERS64 pNTHeader = RtlImageNtHeader(DllBase);
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD64)pNTHeader + sizeof(IMAGE_NT_HEADERS64));
	ULONG nAlign = pNTHeader->OptionalHeader.SectionAlignment;
	SIZE_T ImageSize = (pNTHeader->OptionalHeader.SizeOfHeaders + nAlign - 1) / nAlign * nAlign;
	for (int i = 0; i < pNTHeader->FileHeader.NumberOfSections; ++i)
	{
		int CodeSize = pSectionHeader[i].Misc.VirtualSize;
		int LoadSize = pSectionHeader[i].SizeOfRawData;
		int MaxSize = (LoadSize > CodeSize) ? (LoadSize) : (CodeSize);
		int SectionSize = (pSectionHeader[i].VirtualAddress + MaxSize + nAlign - 1) / nAlign * nAlign;
		if (ImageSize < SectionSize)
			ImageSize = SectionSize;
	}
	return ImageSize;
}