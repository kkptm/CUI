#pragma once
#include <Windows.h>
#include <vector>
#include <string>

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
#include "TcpSocket.h"
#include "FileStream.h"
#include "Dictionary.h"
#include "HttpHelper.h"
#include "Environment.h"
#include "StringHelper.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#ifndef RELOC
#define RELOC(p,o) (void*)((char*)p ? (((char*)p + o + 4) + (*(int*)((char*)p + o))) : NULL)
#endif

#ifndef typeof
#define typeof(x) decltype(x)
#endif

#ifndef PROPERTY
#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n)) t n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n
#define GET(t,n) t property__get_##n() 
#define SET(t,n) void property__set_##n(t value)
#define _readonly_property(t,n) READONLY_PROPERTY(t,n);GET(t,n)
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
INT64 GetTick();
