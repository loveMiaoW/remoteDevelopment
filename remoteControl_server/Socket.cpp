#include "pch.h"
#include "Socket.h"
#include "info.h"

Socket::Socket()
{
}

bool Socket::startUp()
{
	WSADATA wsa;
	int ret = WSAStartup(MAKEWORD(2,2),&wsa);
	if (ret != 0) 
	{
		return false;
	}
	return true;
}

bool Socket::createSocket()
{
	m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) 
	{
		return false;
	}
	return true;
}

bool Socket::bindSocket()
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	 
	int ret = ::bind(m_socket,(sockaddr*)&addr,sizeof(addr));

	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool Socket::listenSocket()
{
	int ret = ::listen(m_socket,SOMAXCONN);
	if (ret == SOCKET_ERROR) 
	{
		return false;
	}
	return true;
}

SOCKET Socket::acceptSock(SOCKADDR_IN& _pi)
{
	int m_size = sizeof(_pi);
	return ::accept(m_socket,(sockaddr*)&_pi,&m_size);
}

bool Socket::startSock()
{
	if (startUp() == false) 
	{
		AfxMessageBox(_T("³õÊ¼»¯Ê§°Ü"));
		return false;
	}

	if (createSocket() == false)
	{
		AfxMessageBox(_T("´´½¨SOCKETÊ§°Ü"));
		return false;
	}

	if (bindSocket() == false)
	{
		AfxMessageBox(_T("°ó¶¨SOCKETÊ§°Ü"));
		return false;
	}

	if (listenSocket() == false)
	{
		AfxMessageBox(_T("¼àÌýSOCKETÊ§°Ü"));
		return false;
	}
	HANDLE pthread = CreateThread(NULL,0, acceptThread,this,0,NULL);
	return true;
}

DWORD __stdcall Socket::acceptThread(LPVOID lparam)
{
	Socket* pthis = (Socket*)lparam;
	PSOCKADDR_IN si = new SOCKADDR_IN();
	ZeroMemory(si, sizeof(SOCKADDR_IN));
	pthis->m_client_socket = pthis->acceptSock(*si);
	if (pthis->m_client_socket != INVALID_SOCKET)
	{
		::PostMessage(pthis->m_hwnd, WM_ACCEPTSUCCESS,(WPARAM)pthis->m_client_socket,(LPARAM)si);
	}

	HANDLE handle = CreateThread(NULL,0,recvThread,pthis,0,NULL);
	
	return 0;	
}

DWORD __stdcall Socket::recvThread(LPVOID lparam)
{
	Socket* pthis = (Socket*)lparam;
	while (true)
	{
		HEAD_INFO head;
		pthis->recvMess(pthis->m_client_socket,(char*)&head,sizeof(head));
		pthis->judge(head);
	}
	return 0;
}

void Socket::setHw(HWND _hwnd)
{
	m_hwnd = _hwnd;
}

bool Socket::sendMess(SOCKET _sock, const char* _mes, int _len)
{
	int ret = ::send(_sock, _mes, _len, 0);
	
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool Socket::recvMess(SOCKET _sock, char* _mes, int _len)
{
	int ret = recv(_sock,_mes,_len,0);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

void Socket::judge(HEAD_INFO head)
{
	if (head.commend == ON_CMDDATA)
	{
		judgeCmdData(head);
	}
}

void Socket::judgeCmdData(HEAD_INFO head)
{
	char* buff = new char[head.len];
	ZeroMemory(buff,head.len);
	recvMess(m_client_socket,buff,head.len);
	PostMessage(m_hwnd,WM_CMDDATA,(WPARAM)buff,NULL);
}

Socket::~Socket()
{

}
