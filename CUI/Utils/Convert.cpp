#include "Convert.h"

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
__forceinline char* TCHAR2char(const wchar_t* STR)
{
	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);
	char* str = new char[sizeof(char) * size];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, str, size, NULL, FALSE);
	return str;
}
__forceinline wchar_t* Char2TCHAR(const char* _char)
{
	int size = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	wchar_t* tchar = new wchar_t[sizeof(wchar_t) * size];
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, size);
	return tchar;
}
std::string Convert::AnsiToUtf8(const std::string str) {
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	len = ::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* utf8 = new char[len];
	::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	std::string result(utf8);
	delete[] wstr;
	delete[] utf8;
	return result;
}
std::string Convert::GbkToUtf8(const std::string str) {
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	len = ::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* utf8 = new char[len];
	::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	std::string result(utf8);
	delete[] wstr;
	delete[] utf8;
	return result;
}
std::string Convert::Gb2312ToUtf8(const std::string str) {
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	len = ::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* utf8 = new char[len];
	::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	std::string result(utf8);
	delete[] wstr;
	delete[] utf8;
	return result;
}
std::string Convert::Utf8ToAnsi(const std::string str) {
	int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
	len = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* ansi = new char[len];
	::WideCharToMultiByte(CP_ACP, 0, wstr, -1, ansi, len, NULL, NULL);
	std::string result(ansi);
	delete[] wstr;
	delete[] ansi;
	return result;
}
std::string Convert::Utf8ToGbk(const std::string str) {
	int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
	len = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* gbk = new char[len];
	::WideCharToMultiByte(CP_ACP, 0, wstr, -1, gbk, len, NULL, NULL);
	std::string result(gbk);
	delete[] wstr;
	delete[] gbk;
	return result;
}
std::string Convert::Utf8ToGb2312(const std::string str) {
	int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
	len = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* gb2312 = new char[len];
	::WideCharToMultiByte(CP_ACP, 0, wstr, -1, gb2312, len, NULL, NULL);
	std::string result(gb2312);
	delete[] wstr;
	delete[] gb2312;
	return result;
}
std::string Convert::Utf16ToUtf8(const std::wstring wstr) {
	int len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* utf8 = new char[len];
	::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, utf8, len, NULL, NULL);
	std::string result(utf8);
	delete[] utf8;
	return result;
}
std::wstring Convert::Utf8ToUtf16(const std::string str) {
	int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
	std::wstring result(wstr);
	delete[] wstr;
	return result;
}
std::wstring Convert::AnsiToUnicode(const std::string ansiStr)
{
	int len = MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), ansiStr.length(), NULL, 0);
	wchar_t* unicodeStr = new wchar_t[len + 1];
	memset(unicodeStr, 0, (len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), ansiStr.length(), unicodeStr, len);
	std::wstring result(unicodeStr);
	delete[] unicodeStr;
	return result;
}
std::string Convert::UnicodeToAnsi(const std::wstring unicodeStr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, unicodeStr.c_str(), unicodeStr.length(), NULL, 0, NULL, NULL);
	char* ansiStr = new char[len + 1];
	memset(ansiStr, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, unicodeStr.c_str(), unicodeStr.length(), ansiStr, len, NULL, NULL);
	std::string result(ansiStr);
	delete[] ansiStr;
	return result;
}
std::wstring Convert::Utf8ToUnicode(const std::string utf8Str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), utf8Str.length(), NULL, 0);
	wchar_t* unicodeStr = new wchar_t[len + 1];
	memset(unicodeStr, 0, (len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), utf8Str.length(), unicodeStr, len);
	std::wstring result(unicodeStr);
	delete[] unicodeStr;
	return result;
}
std::string Convert::UnicodeToUtf8(const std::wstring unicodeStr)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, unicodeStr.c_str(), unicodeStr.length(), NULL, 0, NULL, NULL);
	char* utf8Str = new char[len + 1];
	memset(utf8Str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicodeStr.c_str(), unicodeStr.length(), utf8Str, len, NULL, NULL);
	std::string result(utf8Str);
	delete[] utf8Str;
	return result;
}
std::string Convert::wstring_to_string(const std::wstring wstr)
{
	char* c = TCHAR2char(wstr.c_str());;
	std::string s = c;
	delete[] c;
	return s;
}
std::wstring Convert::string_to_wstring(const std::string str)
{
	wchar_t* wc = Char2TCHAR(str.c_str());;
	std::wstring ws = wc;
	delete[] wc;
	return ws;
}
std::string Convert::ToBase64(const std::string input) {
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";
	std::string output;
	int char_count = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	for (const auto& c : input) {
		char_array_3[char_count++] = c;
		if (char_count == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for (const auto& i : char_array_4) {
				output += base64_chars[i];
			}
			char_count = 0;
		}
	}
	if (char_count) {
		for (auto i = char_count; i < 3; i++) {
			char_array_3[i] = '\0';
		}
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for (auto i = 0; i < char_count + 1; i++) {
			output += base64_chars[char_array_4[i]];
		}
		while (char_count++ < 3) {
			output += '=';
		}
	}
	return output;
}
std::string Convert::ToBase64(const std::vector<BYTE>& input)
{
	return ToBase64(std::string((char*)input.data(), input.size()));
}
std::string Convert::FromBase64(const std::string input) {
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";
	std::string output;
	int char_count = 0;
	unsigned char char_array_4[4];
	unsigned char char_array_3[3];
	for (const auto& c : input) {
		if (c == '=') {
			break;
		}
		char_array_4[char_count++] = base64_chars.find(c);
		if (char_count == 4) {
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			std::copy(char_array_3, char_array_3 + 3, std::back_inserter(output));
			char_count = 0;
		}
	}
	if (char_count) {
		for (auto i = char_count; i < 4; i++) {
			char_array_4[i] = 0;
		}
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		std::copy(char_array_3, char_array_3 + char_count - 1, std::back_inserter(output));
	}
	return output;
}
std::vector<BYTE> Convert::FromBase64ToBytes(const std::string input)
{
	auto tmp = FromBase64(input);
	BYTE* dta = (BYTE*)tmp.data();
	return std::vector<BYTE>(dta, dta + tmp.size());
}
int Convert::ToInt32(const std::string input)
{
	return atoi(input.c_str());
}
__int64 Convert::ToInt64(const std::string input)
{
	return _atoi64(input.c_str());
}
double Convert::ToFloat(const std::string input)
{
	return atof(input.c_str());
}