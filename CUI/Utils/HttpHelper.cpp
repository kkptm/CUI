#include "HttpHelper.h"
#pragma comment(lib, "wininet.lib")
#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
std::string HttpHelper::CheckUrl(std::string input)
{
    auto replace = [](std::string& str, const char* old_str, const char* new_str) {
        size_t len_old = strlen(old_str);
        size_t len_new = strlen(new_str);
        size_t pos = str.find(old_str);
        while (pos != std::string::npos) {
            str.replace(pos, len_old, new_str);
            pos = str.find(old_str, pos + len_new);
        }
        };
    replace(input, " ", "%20");
    replace(input, "\"", "%22");
    replace(input, "#", "%23");
    replace(input, "%", "%25");
    replace(input, "&", "%26");
    replace(input, "(", "%28");
    replace(input, ")", "%29");
    replace(input, "+", "%2B");
    replace(input, ",", "%2C");
    replace(input, "/", "%2F");
    replace(input, ":", "%3A");
    replace(input, ";", "%3B");
    replace(input, "<", "%3C");
    replace(input, "=", "%3D");
    replace(input, ">", "%3E");
    replace(input, "?", "%3F");
    replace(input, "@", "%40");
    replace(input, "\\", "%5C");
    replace(input, "|", "%7C");
    return input;
}

std::string HttpHelper::GetHostNameFromURL(const std::string& url) {
    std::string hostName;
    size_t pos = url.find("//");
    if (pos != std::string::npos) {
        pos += 2;
        size_t endPos = url.find('/', pos);
        if (endPos != std::string::npos) {
            hostName = url.substr(pos, endPos - pos);
        }
        else {
            hostName = url.substr(pos);
        }
    }
    return hostName;
}
std::string HttpHelper::HttpGet(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("HTTPGET", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) {
        return "";
    }

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return "";;
    }
    std::string response = "";
    char szBuffer[0x1000];
    DWORD dwReadSize = 0;
    response.clear();
    while (InternetReadFile(hUrl, szBuffer, sizeof(szBuffer), &dwReadSize) && dwReadSize > 0) {
        response.append(szBuffer, dwReadSize);
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return response;
}
std::string HttpHelper::HttpGetStream(const std::string& url, HTTP_STREAM_CALLBACK callback) {
    HINTERNET hInternet = InternetOpenA("HTTPGET", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) {
        return "";
    }

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string response = "";
    char szBuffer[0x1000];
    DWORD dwReadSize = 0;
    while (InternetReadFile(hUrl, szBuffer, sizeof(szBuffer), &dwReadSize) && dwReadSize > 0) {
        std::string str = std::string(szBuffer, dwReadSize);
        if (callback) callback(str);
        response += str;
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return response;
}
std::string HttpHelper::HttpPost(const std::string& url, const std::string& api, const std::string& data, int port)
{
    HINTERNET hInternet = InternetOpenA("HTTPPOST", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet)
    {
        return "";
    }

    HINTERNET hConnect = InternetConnectA(hInternet, url.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect)
    {
        InternetCloseHandle(hInternet);
        return "";
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", api.c_str(), NULL, NULL, NULL, 0, 0);
    if (!hRequest)
    {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string headers = "Content-Type: application/x-www-form-urlencoded\r\n";
    std::string request = data;

    BOOL bResult = HttpSendRequestA(hRequest, headers.c_str(), headers.length(), (LPVOID)request.c_str(), request.length());
    if (!bResult)
    {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string response;
    char buffer[1024];
    DWORD bytesRead = 0;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}
std::string HttpHelper::HttpPostStream(const std::string& url, const std::string& api, const std::string& data, HTTP_STREAM_CALLBACK callback, int port)
{
    HINTERNET hInternet = InternetOpenA("HTTPPOST", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet)
    {
        return "";
    }

    HINTERNET hConnect = InternetConnectA(hInternet, url.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect)
    {
        InternetCloseHandle(hInternet);
        return "";
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", api.c_str(), NULL, NULL, NULL, 0, 0);
    if (!hRequest)
    {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string headers = "Content-Type: application/x-www-form-urlencoded\r\n";
    std::string request = data;

    BOOL bResult = HttpSendRequestA(hRequest, headers.c_str(), headers.length(), (LPVOID)request.c_str(), request.length());
    if (!bResult)
    {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string response;
    char buffer[1024];
    DWORD bytesRead = 0;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        if (callback) callback(std::string(buffer, bytesRead));
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}

std::string HttpHelper::HTTPSGET(const std::string& url)
{
    HINTERNET hInternet = InternetOpenA("HTTPSGET", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        return "";
    }

    HINTERNET hConnect = InternetConnectA(hInternet, GetHostNameFromURL(url).c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
    if (hConnect == NULL) {
        InternetCloseHandle(hInternet);
        return "";
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", url.c_str(), HTTP_VERSIONA, "", NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID, 0);
    if (hRequest == NULL) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string response;
    if (HttpSendRequestA(hRequest, NULL, 0, NULL, 0)) {
        char buffer[1024];
        DWORD bytesRead = 0;
        while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
            response.append(buffer, bytesRead);
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}
std::string HttpHelper::HTTPSPOST(const std::string& url, const std::string& data) {
    HINTERNET hInternet = InternetOpenA("HTTPSPOST", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        return "";
    }

    HINTERNET hConnect = InternetConnectA(hInternet, GetHostNameFromURL(url).c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
    if (hConnect == NULL) {
        InternetCloseHandle(hInternet);
        return "";
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", url.c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID, 1);
    if (hRequest == NULL) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string response;
    if (HttpSendRequest(hRequest, NULL, 0, const_cast<char*>(data.c_str()), data.length())) {
        char buffer[1024];
        DWORD bytesRead = 0;
        while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
            response.append(buffer, bytesRead);
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}