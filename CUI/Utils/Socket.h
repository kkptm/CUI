#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "defines.h"
#include <iostream>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")
class TCPSocket {
public:
	SOCKET Handle;
	TCPSocket();
	TCPSocket(SOCKET h);
	~TCPSocket();
	bool Connect(const char* ip, int port);
	bool Listen(int port, int backlog = SOMAXCONN);
	TCPSocket* Accept();
	int Send(const char* data, int length);
	int Receive(char* buffer, int length);
	std::string GetRemoteIP();
	int GetRemotePort();
	void Close();
	bool IsConnected();
};
class UDPSocket {
public:
	SOCKET Handle;

	UDPSocket();
	~UDPSocket();
	bool Bind(int port);
	void Unbind();
	int SendTo(const char* data, int length, const char* ip, int port);
	int ReceiveFrom(char* buffer, int length, std::string& fromIP, int& fromPort);
	void Close();
};