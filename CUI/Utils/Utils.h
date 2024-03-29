#pragma once
#include "defines.h"
#include <vector>
#include <string>

#include "StringBuilder.h"
#include "List.h"
#include "File.h"
#include "Guid.h"
#include "Tuple.h"
#include "Dialog.h"
#include "Convert.h"
#include "Process.h"
#include "CRandom.h"
#include "FileInfo.h"
#include "DateTime.h"
#include "Registry.h"
#include "Socket.h"
#include "FileStream.h"
#include "Dictionary.h"
#include "HttpHelper.h"
#include "Environment.h"
#include "StringHelper.h"
#include "Thread.h"
#include "DataPack.h"
#include "Clipboard.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#ifndef RELOC
#define RELOC(p,o) (void*)((char*)p ? (((char*)p + o + 4) + (*(int*)((char*)p + o))) : NULL)
#endif

DWORD NtBuildVersion();
char* ToHex(const char* data, size_t len);
char* ToHex(const BYTE* data, size_t len);
std::vector<BYTE> ParseHex(const char* hex);
std::vector<__int16> ParserPattern(const char* text);
void* FindPattern(const char* szModule, const char* sPattern, int offset = 0);
std::vector<void*> FindAllPattern(const char* szModule, const char* sPattern, int offset = 0);
void* FindPattern(void* _begin, const char* sPattern, int search_size, int offset = 0);
std::vector<void*> FindAllPattern(void* _begin, const char* sPattern, int search_size, int offset = 0);
void PrintHex(void* ptr, int count, int splitLine);
void PrintHex(void* ptr, int count);
__int64 GetTick();
void MakePermute(std::vector<int> nums, std::vector<std::vector<int>>& result, int start = 0);

std::string GetLastErrorMessage();
PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID Base);
SIZE_T GetSectionSize(_In_ PVOID DllBase);