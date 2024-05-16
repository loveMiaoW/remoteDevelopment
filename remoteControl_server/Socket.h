#pragma once
#include "info.h"
class Socket
{
public:
	Socket();
	~Socket();


public:
	bool startUp();
	bool createSocket();
	bool bindSocket();
	bool listenSocket();

	SOCKET acceptSock(SOCKADDR_IN &_pi);
	bool startSock();
	static DWORD WINAPI acceptThread(LPVOID lparam);
	static DWORD WINAPI recvThread(LPVOID lparam);
	void setHw(HWND _hwnd);
	bool sendMess(SOCKET _sock, const char* _mes, int _len);
	bool recvMess(SOCKET _sock, char* _mes, int _len);
	void judge(HEAD_INFO head);
	void judgeCmdData(HEAD_INFO head);
public:
	SOCKET m_socket;
	SOCKET m_client_socket;
	HWND m_hwnd;
	
};

