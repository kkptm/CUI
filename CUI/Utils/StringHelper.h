#pragma once
#include <string>
#include <vector>
class StringHelper
{
public:
	static std::vector<std::string> Split(std::string str, std::string separator);
	static std::vector<std::string> Split(std::string str, std::initializer_list<std::string> separators);
	static std::vector<std::wstring> Split(std::wstring str, std::wstring separator);
	static std::vector<std::wstring> Split(std::wstring str, std::initializer_list<std::wstring> separators);
	static std::string Replace(std::string str, std::string oldstr, std::string newstr);
	static std::wstring Replace(std::wstring str, std::wstring oldstr, std::wstring newstr);
	static std::string ToUpper(std::string str);
	static std::wstring ToUpper(std::wstring str);
	static std::string ToLower(std::string str);
	static std::wstring ToLower(std::wstring str);
	static bool StartsWith(std::string str, std::string prefix);
	static bool StartsWith(std::wstring str, std::wstring prefix);
	static bool EndsWith(std::string str, std::string suffix);
	static bool EndsWith(std::wstring str, std::wstring suffix);
	static std::string Trim(std::string str);
	static std::wstring Trim(std::wstring str);
	static std::string TrimLeft(std::string str);
	static std::wstring TrimLeft(std::wstring str);
	static std::string TrimRight(std::string str);
	static std::wstring TrimRight(std::wstring str);
	static int IndexOf(std::string str, std::string substr);
	static int IndexOf(std::wstring str, std::wstring substr);
	static int LastIndexOf(std::string str, std::string substr);
	static int LastIndexOf(std::wstring str, std::wstring substr);
	static bool Contains(std::string str, std::string substr);
	static bool Contains(std::wstring str, std::wstring substr);
	static int GetHashCode(std::string str);
	static int GetHashCode(std::wstring str);
	static std::string Insert(std::string str, int index, std::string substr);
	static std::wstring Insert(std::wstring str, int index, std::wstring substr);
	static std::string Remove(std::string str, int index, int count);
	static std::wstring Remove(std::wstring str, int index, int count);
	static std::string Join(std::vector<std::string> strs, std::string separator);
	static std::wstring Join(std::vector<std::wstring> strs, std::wstring separator);
	template <class... _Types>
	static inline std::wstring Format(const wchar_t* fmt, _Types&&... _Args)
	{
		int size = swprintf(nullptr, 0, fmt, _Args...) + 1;
		std::wstring result(size, L'\0');
		swprintf_s(&result[0], size, fmt, _Args...);
		return result;
	}
	template <class... _Types>
	static inline std::string Format(const char* fmt, _Types&&... _Args)
	{
		int size = snprintf(nullptr, 0, fmt, _Args...) + 1;
		std::string result(size, '\0');
		sprintf_s(&result[0], size, fmt, _Args...);
		return result;
	}
};