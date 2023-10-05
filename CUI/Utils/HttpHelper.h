#pragma once
#include <Windows.h>
#include <string>
#include <wininet.h>
#include <Winhttp.h>
typedef void(*HTTP_STREAM_CALLBACK)(std::string);
class HttpHelper
{
public:
    static std::string CheckUrl(std::string input);
    static std::string GetHostNameFromURL(const std::string& url);
    static std::string HttpGet(const std::string& url);
    static std::string HttpGetStream(const std::string& url, HTTP_STREAM_CALLBACK callback = NULL);
    //string result = HttpPost("www.example.com","/api/example",data);
    static std::string HttpPost(const std::string& url, const std::string& api, const std::string& data, int port = INTERNET_DEFAULT_HTTP_PORT);
    static std::string HttpPostStream(const std::string& url, const std::string& api, const std::string& data, HTTP_STREAM_CALLBACK callback = NULL, int port = INTERNET_DEFAULT_HTTP_PORT);
    static std::string HTTPSGET(const std::string& url);
    static std::string HTTPSPOST(const std::string& url, const std::string& data);
};

