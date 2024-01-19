#pragma once
#include "defines.h"
#include <vector>
#include <string>
#include <codecvt>
class Convert {
public:
	static std::string AnsiToUtf8(const std::string str);
	static std::string GbkToUtf8(const std::string str);
	static std::string Gb2312ToUtf8(const std::string str);
	static std::string Utf8ToAnsi(const std::string str);
	static std::string Utf8ToGbk(const std::string str);
	static std::string Utf8ToGb2312(const std::string str);
	static std::string Utf16ToUtf8(const std::wstring wstr);
	static std::wstring Utf8ToUtf16(const std::string str);
	static std::wstring AnsiToUnicode(const std::string ansiStr);
	static std::string UnicodeToAnsi(const std::wstring unicodeStr);
	static std::wstring Utf8ToUnicode(const std::string utf8Str);
	static std::string UnicodeToUtf8(const std::wstring unicodeStr);
	static std::string wstring_to_string(const std::wstring wstr);
	static std::wstring string_to_wstring(const std::string str);
	static std::string ToBase64(const std::string input);
	static std::string ToBase64(const std::vector<BYTE>& input);
	static std::string FromBase64(const std::string input);
	static std::vector<BYTE> FromBase64ToBytes(const std::string input);
	static int ToInt32(const std::string input);
	static __int64 ToInt64(const std::string input);
	static double ToFloat(const std::string input);
#define ToDouble ToFloat
};

