#include "File.h"
#include <fstream>
#include <filesystem>
#include "StringHelper.h"
#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
bool File::Exists(const std::string path)
{
	return std::filesystem::exists(path);
}
void File::Delete(const std::string path)
{
	if (File::Exists(path))
	{
		std::filesystem::remove(path);
	}
}
void File::Copy(const std::string src, const std::string dest)
{
	std::filesystem::copy(src, dest);
}
void File::Move(const std::string src, const std::string dest)
{
	std::filesystem::rename(src, dest);
}
void File::Create(const std::string path)
{
	std::ofstream fout(path);
	fout.close();
}
std::string File::ReadAllText(const std::string path)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return {};
	}
	DWORD FileSize = GetFileSize(pFile, NULL);
	std::string result(FileSize, '\0');
	DWORD dwBytesRead = 0;
	BYTE* buffer = (BYTE*)result.data();
	if (!ReadFile(pFile, buffer, result.size() - dwBytesRead, &dwBytesRead, NULL)) {
		result.resize(dwBytesRead);
	}
	CloseHandle(pFile);
	return result;
}
std::vector<BYTE> File::ReadAllBytes(const std::string path)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return {};
	}
	DWORD FileSize = GetFileSize(pFile, NULL);
	std::vector<BYTE> result(FileSize);
	DWORD dwBytesRead = 0;
	BYTE* buffer = result.data();
	if (!ReadFile(pFile, buffer, result.size() - dwBytesRead, &dwBytesRead, NULL)) {
		result.resize(dwBytesRead);
	}
	CloseHandle(pFile);
	return result;
}
std::vector<std::string> File::ReadAllLines(const std::string path)
{
	auto str = File::ReadAllText(path);
	return StringHelper::Split(str, "\n");
}
void File::WriteAllText(const std::string path, const std::string content)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	DWORD dwBytesWritten = 0;
	WriteFile(pFile, content.c_str(), content.size(), &dwBytesWritten, NULL);
	CloseHandle(pFile);
}
void File::WriteAllBytes(const std::string path, const std::vector<BYTE> content)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	DWORD dwBytesWritten = 0;
	WriteFile(pFile, content.data(), content.size(), &dwBytesWritten, NULL);
	CloseHandle(pFile);
}

void File::WriteAllBytes(const std::string path, const BYTE* content, size_t size)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	DWORD dwBytesWritten = 0;
	WriteFile(pFile, content, size, &dwBytesWritten, NULL);
	CloseHandle(pFile);
}
void File::WriteAllLines(const std::string path, const std::vector<std::string> content)
{
	auto str = StringHelper::Join(content, "\n");
	File::WriteAllText(path, str);
}
void File::AppendAllText(const std::string path, const std::string content)
{
	HANDLE pFile = CreateFileA(path.c_str(), FILE_APPEND_DATA, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	DWORD dwBytesWritten = 0;
	WriteFile(pFile, content.c_str(), content.size(), &dwBytesWritten, NULL);
	CloseHandle(pFile);
}
void File::AppendAllBytes(const std::string path, const std::vector<BYTE> content)
{
	HANDLE pFile = CreateFileA(path.c_str(), FILE_APPEND_DATA, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	DWORD dwBytesWritten = 0;
	WriteFile(pFile, content.data(), content.size(), &dwBytesWritten, NULL);
	CloseHandle(pFile);
}
void File::AppendAllLines(const std::string path, const std::vector<std::string> content)
{
	auto str = StringHelper::Join(content, "\n");
	File::AppendAllText(path, str);
}
void File::SetAttributes(const std::string path, FileAttributes attributes)
{
	SetFileAttributesA(path.c_str(), (DWORD)attributes);
}
FileAttributes File::GetAttributes(const std::string path)
{
	return (FileAttributes)GetFileAttributesA(path.c_str());
}
void File::SetCreationTime(const std::string path, FILETIME time)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	SetFileTime(pFile, &time, NULL, NULL);
	CloseHandle(pFile);
}
FILETIME File::GetCreationTime(const std::string path)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return {};
	}
	FILETIME time;
	GetFileTime(pFile, &time, NULL, NULL);
	CloseHandle(pFile);
	return time;
}
void File::SetLastAccessTime(const std::string path, FILETIME time)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	SetFileTime(pFile, NULL, &time, NULL);
	CloseHandle(pFile);
}
FILETIME File::GetLastAccessTime(const std::string path)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return {};
	}
	FILETIME time;
	GetFileTime(pFile, NULL, &time, NULL);
	CloseHandle(pFile);
	return time;
}
void File::SetLastWriteTime(const std::string path, FILETIME time)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return;
	}
	SetFileTime(pFile, NULL, NULL, &time);
	CloseHandle(pFile);
}
FILETIME File::GetLastWriteTime(const std::string path)
{
	HANDLE pFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE) {
		CloseHandle(pFile);
		return {};
	}
	FILETIME time;
	GetFileTime(pFile, NULL, NULL, &time);
	CloseHandle(pFile);
	return time;
}