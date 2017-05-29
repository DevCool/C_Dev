/*****************************************************
 * KillPower 2 - Broadcasting Killpower
 *****************************************************
 * Copyright (C) 2017, Philip Simonson.
 *****************************************************
 */

#if defined(_WIN32) || (_WIN64)
	/* windows header files */
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <windows.h>
	#include <tlhelp32.h>
#endif

/* standard header files */
#include <stdio.h>
#include <string.h>

/* redefine size_t as what it should be,
 * just in case I haven't included stddef.h
 */
typedef unsigned long long int size_t;

#if defined(_WIN32) || (_WIN64)
BOOL MByteToUnicode(LPCSTR mbStr, LPWSTR uStr, DWORD size)
{
	DWORD minSize;
	minSize = MultiByteToWideChar(CP_ACP, 0, mbStr, -1, NULL, 0);
	if(size < minSize) {
		return FALSE;
	}

	/* Convert string from multi-byte to unicode. */
	MultiByteToWideChar(CP_ACP, 0, mbStr, -1, uStr, minSize);
	return TRUE;
}

BOOL UnicodeToMByte(LPCWSTR uStr, LPSTR mbStr, DWORD size)
{
	DWORD minSize;
	minSize = WideCharToMultiByte(CP_OEMCP, 0, uStr, -1, NULL, 0, NULL, FALSE);
	if(size < minSize) {
		return FALSE;
	}

	/* Convert string from Unicode to multibyte */
	WideCharToMultiByte(CP_OEMCP, 0, uStr, -1, mbStr, size, NULL, FALSE);
	return TRUE;
}

DWORD FindProcessId(LPCSTR lpszProcessName, HANDLE *hProcess)
{
	PROCESSENTRY32 peInfo;
	HANDLE procSnapshot;
	peInfo.dwSize = sizeof(peInfo);
	procSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(procSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	if(Process32First(procSnapshot, &peInfo))
		while(Process32Next(procSnapshot, &peInfo)) {
			if(strcmp(lpszProcessName, peInfo.szExeFile) == 0) {
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,peInfo.th32ProcessID);
				printf("Process ID: %lu\n", peInfo.th32ProcessID);
				break;
			}
		}

	CloseHandle(procSnapshot);
	return 0;
}
#endif

void talker(int sockfd, struct addrinfo *server, char *message)
{
	char buffer[BUFSIZ];
	struct sockaddr_in serv = *((struct sockaddr_in*)(server->ai_addr));
	int addrlen = sizeof(serv);
	int bytes;

	bytes = sendto(sockfd, message, strlen(message), 0,
			server->ai_addr, server->ai_addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Couldn't send %d bytes to %s.\n",
			bytes, inet_ntoa(serv.sin_addr));
	} else {
		printf("%d bytes sent to %s.\n", bytes,
			inet_ntoa(serv.sin_addr));
	}

	memset(buffer, 0, sizeof(buffer));
	bytes = recvfrom(sockfd, buffer, BUFSIZ-1, 0,
			(struct sockaddr*)&serv, &addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Lost %d bytes from %s.\n",
			bytes, inet_ntoa(serv.sin_addr));
	} else {
		printf("%d bytes received from %s.\nClient: %s\n",
			bytes, inet_ntoa(serv.sin_addr), buffer);
	}
}

void server_talker(int sockfd, struct sockaddr_in *client)
{
	int addrlen = sizeof(*client);
	char message[BUFSIZ];
	int bytes;

	memset(message, 0, sizeof(message));
	bytes = recvfrom(sockfd, message, BUFSIZ-1, 0,
			(struct sockaddr*)client, &addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Lost %d bytes from %s.\n",
			bytes, inet_ntoa(client->sin_addr));
	} else {
		printf("%d bytes received from %s.\nClient: %s\n",
			bytes, inet_ntoa(client->sin_addr), message);
	}

	bytes = sendto(sockfd, message, strlen(message), 0,
			(struct sockaddr*)client, addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Couldn't send %d bytes to %s.\n",
			bytes, inet_ntoa(client->sin_addr));
	} else {
		printf("%d bytes sent to %s.\n", bytes,
			inet_ntoa(client->sin_addr));
	}
}
