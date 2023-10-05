#include "TcpSocket.h"
#include <string>
#include <sstream>
#include <cstring>
#include <stdexcept>

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
TCPSocket::TCPSocket() :Handle(NULL) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR) {
        std::ostringstream oss;
        oss << "WSAStartup failed with error: " << result;
        throw std::runtime_error(oss.str());
    }
    Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Handle == INVALID_SOCKET) {
        std::ostringstream oss;
        oss << "Socket creation failed with error: " << WSAGetLastError();
        throw std::runtime_error(oss.str());
    }
}
TCPSocket::TCPSocket(SOCKET h) :Handle(h) {}
TCPSocket::~TCPSocket() {
    int result = closesocket(Handle);
    if (result == SOCKET_ERROR) {
        std::cerr << "Socket closing failed with error: " << WSAGetLastError() << std::endl;
    }
    WSACleanup();
}
bool TCPSocket::Connect(const char* ip, int port) {
    sockaddr_in address_;
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = inet_addr(ip);
    address_.sin_port = htons(port);
    int result = ::connect(Handle, (sockaddr*)&address_, sizeof(address_));
    if (result == SOCKET_ERROR) {
        std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}
bool TCPSocket::Listen(int port, int backlog) {
    sockaddr_in address_;
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons(port);
    int result = bind(Handle, (sockaddr*)&address_, sizeof(address_));
    if (result == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    result = ::listen(Handle, backlog);
    if (result == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}
TCPSocket* TCPSocket::Accept() {
    SOCKET client = ::accept(Handle, NULL, NULL);
    if (client == INVALID_SOCKET) {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        return nullptr;
    }
    return new TCPSocket(client);
}
int TCPSocket::Send(const char* data, int length) {
    int result = ::send(Handle, data, length, 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
        return -1;
    }
    return result;
}
int TCPSocket::Receive(char* buffer, int length) {
    int result = ::recv(Handle, buffer, length, 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
        return -1;
    }
    return result;
}
std::string TCPSocket::GetRemoteIP() {
    sockaddr_in addr;
    int len = sizeof(addr);
    if (getpeername(Handle, (sockaddr*)&addr, &len) != SOCKET_ERROR) {
        return inet_ntoa(addr.sin_addr);
    }
    return "";
}
int TCPSocket::GetRemotePort() {
    sockaddr_in addr;
    int len = sizeof(addr);
    if (getpeername(Handle, (sockaddr*)&addr, &len) != SOCKET_ERROR) {
        return ntohs(addr.sin_port);
    }
    return -1;
}
void TCPSocket::Close() {
    int result = closesocket(Handle);
    if (result == SOCKET_ERROR) {
        std::cerr << "Socket closing failed with error: " << WSAGetLastError() << std::endl;
    }
}
bool TCPSocket::IsConnected() {
    fd_set writeSet;
    FD_ZERO(&writeSet);
    FD_SET(Handle, &writeSet);

    timeval timeout = { 0, 0 };

    if (select(0, NULL, &writeSet, NULL, &timeout) == SOCKET_ERROR) {
        std::cerr << "Select failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }

    if (FD_ISSET(Handle, &writeSet)) {
        return true;
    }

    return false;
}