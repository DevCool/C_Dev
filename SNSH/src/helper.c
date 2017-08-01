/***************************************************************
 * helper.c - this contains pretty much only stuff to do with  *
 *            a command line interface, for remote machines.   *
 ***************************************************************
 * Created by Philip "5n4k3" Simonson           (2017)         *
 ***************************************************************
 */

/* standard c language header */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

/* my headers */
#include "prs_socket/socket.h"
#include "debug.h"
#include "helper.h"
#include "transfer.h"

/* all linux headers */
#ifdef __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <espeak/speak_lib.h>
#include "espeech.h"
#endif

/* builtin command strings (compared to what you enter) */
char *builtin_str[] = {
  "cd",
  "ls",
  "rm",
  "mkdir",
  "rmdir",
  "touch",
  "type",
  "write",
  "hostup",
  "transfer",
#ifdef __linux__
  "speak",
  "term",
  "pivot",
#endif
  "help",
  "exit"
};

/* builtin command help */
char *builtin_help[] = {
  "change directory to a new one.\r\n",
  "list directory contents.\r\n",
  "delete a file from the system.\r\n",
  "make a directory in the current one.\r\n",
  "delete an empty directory.\r\n",
  "create a blank file.\r\n",
  "displays a text file 20 lines at a time.\r\n",
  "allows you to write text to a file.\r\n",
  "checks if a host is available on specified port.\r\n",
  "transfers data from one computer to another.\r\n",
#ifdef __linux__
  "speaks the text you type.\r\n",
  "launches a command that is not builtin.\r\n",
  "launches a new xterm with telnet.\r\n",
#endif
  "print this message.\r\n",
  "exit back to echo hello name.\r\n"
};

/* builtin_func[]() - builtin functions array of function pointers.
 */
int (*builtin_func[])(int sockfd, char **args) = {
  &cmd_cd,
  &cmd_ls,
  &cmd_rm,
  &cmd_mkdir,
  &cmd_rmdir,
  &cmd_touch,
  &cmd_type,
  &cmd_write,
  &cmd_hostup,
  &cmd_transfer,
#ifdef __linux__
  &cmd_speak,
  &cmd_term,
  &cmd_pivot,
#endif
  &cmd_help,
  &cmd_exit
};

/* cmd_len() - returns the count of all available builtin commands.
 */
int cmd_len(void) {
  return sizeof(builtin_str) / sizeof(char *);
}

/* cmd_cd() - change directory on remote machine.
 */
int cmd_cd(int sockfd, char **args) {
  char data[BUFSIZ];
  
  memset(data, 0, sizeof data);
  if(args[1] == NULL) {
    snprintf(data, sizeof data, "Usage: %s <dirname>\r\n", args[0]);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.");
  } else {
    ERROR_FIXED(chdir(args[1]) != 0, "Could not change to new directory.");
    snprintf(data, sizeof data, "Changed directory to %s\r\n", args[1]);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.");
  }
  return 1;

error:
  return 1;
}

/* cmd_ls() - list directory on remote machine.
 */
int cmd_ls(int sockfd, char **args) {
  DIR *d;
  struct dirent *dir;
  char msg[1024];
  int i;
  
  memset(msg, 0, sizeof msg);
  if(args[1] != NULL)
    d = opendir(args[1]);
  else
    d = opendir(".");
  
  if(d != NULL) {
    memset(msg, 0, sizeof msg);
    if(args[1] != NULL)
      snprintf(msg, sizeof msg, "Directory listing of %s\r\n", args[1]);
    else
      snprintf(msg, sizeof msg, "Directory listing of ./\r\n");
    if(send(sockfd, msg, strlen(msg), 0) != (int)strlen(msg))
      puts("Error: Could not send data to client.");
    i = 0;
    while((dir = readdir(d))) {
      memset(msg, 0, sizeof msg);
      snprintf(msg, sizeof msg, "%s ", dir->d_name);
      send(sockfd, msg, strlen(msg), 0);
      if(i < 4)
	++i;
      else {
	i = 0;
	if(send(sockfd, "\r\n", 2, 0) != 2)
	  puts("Error: Could not send data to client.");
      }
    }
    if(send(sockfd, "\r\n", 2, 0) != 2)
      puts("Error: Could not send data to client.");
    memset(msg, 0, sizeof msg);
    if(closedir(d) != 0) {
      snprintf(msg, sizeof msg, "End of listing.\r\n");
      if(send(sockfd, msg, strlen(msg), 0) != (int)strlen(msg))
	puts("Error: Could not send data to client.");
    }
  } else {
    snprintf(msg, sizeof msg, "Could not list directory, maybe it doesn't exist.\r\n");
    if(send(sockfd, msg, strlen(msg), 0) != (int)strlen(msg))
      puts("Error: Could not send data to client.");
  }
  return 1;
}

/* cmd_rm() - remove a file or many files from remote machine.
 */
int cmd_rm(int sockfd, char **args) {
  char data[BUFSIZ];
  int i = 1;
  
  if(args == NULL || sockfd < 0)
    return -1;
  else if(args[1] == NULL)
    return -1;
  else {
    char msg[BUFSIZ];

    while(args[i] != NULL) {
      if(remove(args[i]) != 0) {
	memset(msg, 0, sizeof msg);
	snprintf(msg, sizeof msg, "Cannot remove file %s\n", args[i]);
	ERROR_FIXED(send(sockfd, msg, strlen(msg), 0) != (int)strlen(data),
		    "Could not send message.");
      } else {
	snprintf(msg, sizeof msg, "File %s removed.\r\n", args[i]);
	ERROR_FIXED(send(sockfd, msg, strlen(msg), 0) != (int)strlen(data),
		    "Could not send message.");
      }
      ++i;
    }
  }
  snprintf(data, sizeof data, "Total files removed %d.\r\n", i-1);
  ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
	      "Could not send message.");
  return 1;

error:
  return 1;
}

/* cmd_mkdir() - makes a directory on the remote machine.
 */
int cmd_mkdir(int sockfd, char **args) {
  char data[BUFSIZ];
  int i = 1;
  
  if(args == NULL || sockfd < 0) {
    return -1;
  } else if(args[1] == NULL) {
    return -1;
  } else {
    while(args[i] != NULL) {
      memset(data, 0, sizeof data);
#if defined(_WIN32) || (_WIN64)
      if(mkdir(args[i]) != 0)
#else
      if(mkdir(args[i], 0755) != 0)
#endif
	snprintf(data, sizeof data, "Directory [%s] not created.\r\n", args[i]);
      else
	snprintf(data, sizeof data, "Created [%s] directory.\r\n", args[i]);
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Could not send data to client.");
      ++i;
    }
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Total directories created: %d\r\n", i-1);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.");
  }
  return 1;

error:
  return 1;
}

/* cmd_rmdir() - removes a directory on the remote machine.
 */
int cmd_rmdir(int sockfd, char **args) {
  char data[BUFSIZ];
  int i = 1;
  
  if(args == NULL || sockfd < 0) {
    return -1;
  } else if(args[1] == NULL) {
    return -1;
  } else {
    while(args[i] != NULL) {
      memset(data, 0, sizeof data);
      if(rmdir(args[i]) != 0)
	snprintf(data, sizeof data, "[%s] : %s.\r\n", args[i], strerror(errno));
      else
	snprintf(data, sizeof data, "[%s] : Removed successfully.\r\n", args[i]);
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Could not send data to client.");
      ++i;
    }
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Total directories removed: %d\r\n", i-1);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.");
  }
  return 1;

error:
  return 1;
}

/* cmd_touch() - creates new files on remote machine.
 */
int cmd_touch(int sockfd, char **args) {
  char data[BUFSIZ];
  int i = 1;
  
  if(args[1] == NULL) {
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Usage: %s file1 file2 ... [files]\r\n", args[0]);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.\n");
  } else {
    while(args[i] != NULL) {
      FILE *fp = NULL;
      memset(data, 0, sizeof data);
      if((fp = fopen(args[i], "wb")) == NULL) {
	snprintf(data, sizeof data, "File [%s] failed to create.\r\n", args[i]);
	ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		    "Could not send data to client.\n");
      } else {
	snprintf(data, sizeof data, "File [%s] created successfully.\r\n", args[i]);
	ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		    "Could not send data to client.\n");
	fclose(fp);
	++i;
      }
    }
  }
  memset(data, 0, sizeof data);
  snprintf(data, sizeof data, "Total count of files created: %d\r\n", i-1);
  ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
	      "Could not send data to client.\n");
  return 1;

error:
  return 1;
}

/* cmd_type - displays a text file 20 lines at a time.
 */
int cmd_type(int sockfd, char **args) {
  FILE *fp = NULL;
  char data[BUFSIZ];

  if(args[1] != NULL) {
    char line[256];
    int count = 0;
    ERROR_FIXED((fp = fopen(args[1], "rt")) == NULL, "Could not open file.");
    memset(data, 0, sizeof(data));
    snprintf(data, sizeof(data), "Type 'quit' and press 'Enter' to stop reading.\r\n"
	     "Press 'Enter' to continue...\r\nFile contents below...\r\n\r\n");
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.\n");
    while(fgets(line, sizeof(line), fp) != NULL) {
      if(strchr(line, '\n') != NULL)
	++count;
      if(count >= 20) {
	char getln[64];
	memset(getln, 0, sizeof(getln));
	ERROR_FIXED(recv(sockfd, getln, sizeof(getln), 0) < 0,
		    "Could not recv data from client.\n");
	if(strcmp(getln, "quit\r\n") == 0)
	  break;
	count = 0;
      }
      ERROR_FIXED(send(sockfd, line, strlen(line), 0) != (int)strlen(line),
		  "Could not send data to client.\n");
    }
    fclose(fp);
  } else {
    memset(data, 0, sizeof(data));
    snprintf(data, sizeof data, "Usage: %s <file.txt>\r\n", args[0]);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.\n");
  }
  return 1;

error:
  if(fp != NULL)
    fclose(fp);
  return -1;
}

/* cmd_write() - allows you to write text to a file.
 */
int cmd_write(int sockfd, char **args) {
  FILE *fp = NULL;
  char data[BUFSIZ];

  if(args[1] != NULL) {
    char line[256];
    int bytes = -1;
    
    ERROR_FIXED((fp = fopen(args[1], "wt")) == NULL, "Could not open file for writing.\n");
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Type 'EOF' on a blank without quotes, to write...\r\n");
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.\n");
    do {
      memset(line, 0, sizeof line);
      memset(data, 0, sizeof data);
      snprintf(data, sizeof data, "> ");
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Could not send data to client.\n");
      ERROR_FIXED((bytes = recv(sockfd, line, sizeof(line), 0)) < 0,
		  "Could not recv data from client.\n");
      if(strncmp(line, "EOF\r\n", sizeof(line)) == 0)
	break;
      else
	fprintf(fp, "%s", line);
    } while(bytes > 0);
    fclose(fp);
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "File written successfully.\r\n");
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.\n");
  } else {
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Usage: %s <file.txt>\r\n", args[0]);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.\n");
  }
  return 1;

error:
  if(fp != NULL)
    fclose(fp);
  return -1;
}

/* cmd_hostup() - checks host status on specific port.
 */
int cmd_hostup(int sockfd, char **args) {
  struct addrinfo hints, *server, *p;
  socklen_t addrlen;
  char data[BUFSIZ];
  int rv, error, clientfd;

  if(args[1] != NULL && args[2] != NULL) {
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(args[1], args[2], &hints, &server) < 0) {
      snprintf(data, sizeof data, "Error: Could not get host info.\r\n");
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Could not send data to client.");
    }
    for(p = server; p != NULL; p = p->ai_next) {
      if((clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	fprintf(stderr, "Error cannot create socket: %s\n", strerror(errno));
	errno = 0;
	continue;
      }

      if((rv = getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &error, &addrlen)) != 0) {
	fprintf(stderr, "Error connecting to socket: %s\n", strerror(rv));
	errno = 0;
	break;
      }

      if(connect(clientfd, p->ai_addr, p->ai_addrlen) < 0) {
	memset(data, 0, sizeof data);
	snprintf(data, sizeof data, "Error: Could not connect to %s on port %d.\r\n",
		 args[1], atoi(args[2]));
	ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		    "Could not send data to client.\n");
	errno = 0;
	goto error;
      }
      break;
    }
    freeaddrinfo(server);
    /* connection successful */
    memset(data, 0, sizeof(data));
    snprintf(data, sizeof(data), "Host: %s\r\nPort: %d\r\nStatus: %s\r\n",
	     args[1], atoi(args[2]), (error) ? "[FAILED]" : "[SUCCESS]");
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.\n");
    close(clientfd);
  } else {
    snprintf(data, sizeof data, "Usage: %s <hostname|ipaddress> <port>\r\n", args[0]);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.");
  }
  return 1;

error:
  if(clientfd > 0)
    close(clientfd);
  return -1;
}

/* cmd_transfer() - download/upload from/to remote machine.
 */
int cmd_transfer(int sockfd, char **args) {
  char data[BUFSIZ];
  int i = 2;

  if(args == NULL || sockfd < 0) {
    return -1;
  } else if(args[0] == NULL) {
    return -1;
  } else if(args[0] != NULL && args[1] == NULL) {
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Usage: %s <upload|download> file1.ext ... [files]\r\n",
	     args[0]);
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Could not send data to client.");
  } else {
    if(strcmp(args[1], "upload") == 0) {
      while(args[i] != NULL) {
	upload("0.0.0.0", args[i]);
	++i;
      }
      memset(data, 0, sizeof data);
      snprintf(data, sizeof data, "Total files downloaded: %d\r\n", i-2);
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Could not send data to client.");
    } else if(strcmp(args[1], "download") == 0) {
      while(args[i] != NULL) {
	download("0.0.0.0", args[i]);
	++i;
      }
      memset(data, 0, sizeof data);
      snprintf(data, sizeof data, "Total files downloaded: %d\r\n", i-2);
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Could not send data to client.");
    } else {
      memset(data, 0, sizeof data);
      snprintf(data, sizeof data, "Usage: %s <upload|download> file1.ext ... [files]\r\n",
	       args[0]);
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Could not send data to client.");
    }
  }
  return 1;

error:
  return -1;
}

#ifdef __linux__
/* cmd_speak() - speaks text you enter on remote machine.
 */
int cmd_speak(int sockfd, char **args) {
  char data[2048];
  char msg[1024];
  int i = 1;

  memset(msg, 0, sizeof msg);
  memset(data, 0, sizeof data);
  if(args == NULL || sockfd < 0)
    return -1;

  snprintf(data, sizeof data, "Speaking: ");
  strncpy(msg, args[i], sizeof msg);
  while(args[++i] != NULL) {
    strncat(msg, " ", sizeof msg);
    strncat(msg, args[i], sizeof msg);
  }
  strncat(data, msg, sizeof data);
  strncat(data, "\r\n", sizeof data);
  ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
	      "Could not send data to client.");
  /* Speak the message */
  speak(msg, strlen(msg));
  return 1;

error:
  return 1;
}

/* cmd_term() - executes an external command.
 */
int cmd_term(int sockfd, char **args) {
  char data[BUFSIZ];
  int pid = 0;

  if(sockfd < 0)
    return 2;
  
  memset(data, 0, sizeof data);
  pid = fork();
  ERROR_FIXED(pid < 0, "Could not fork to background.");
  if(pid == 0) {
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);
    ++args;
    ERROR_FIXED(execvp(*args, args) != (int)strlen(data), "Could not execute command.");
  } else {
    waitpid(0, NULL, 0);
  }
  return 1;

error:
  return -1;
}

/* cmd_pivot() - executes an external command.
 */
int cmd_pivot(int sockfd, char **args) {
  char data[BUFSIZ];
  int pid = 0;

  if(sockfd < 0)
    return 2;
  
  memset(data, 0, sizeof data);
  if(args[0] == NULL) {
    return -1;
  } else if(args[1] == NULL) {
    snprintf(data, sizeof data, "Usage: pivot <ipaddress> <port>\r\n");
    ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Cannot send data to client.");
  } else {
    if(args[1] != NULL && args[2] != NULL) {
      pid = fork();
      ERROR_FIXED(pid < 0, "Could not fork to background.");
      if(pid == 0) {
	dup2(sockfd, 0);
	dup2(sockfd, 1);
	dup2(sockfd, 2);
	ERROR_FIXED(execvp("telnet", args) < 0, "Could not execute command.");
      } else {
	waitpid(0, NULL, 0);
      }
    } else {
      snprintf(data, sizeof data, "Usage: pivot <ipaddress> <port>\r\n");
      ERROR_FIXED(send(sockfd, data, strlen(data), 0) != (int)strlen(data),
		  "Cannot send data to client.");
    }
  }
  return 1;

error:
  return -1;
}
#endif

/* cmd_help() - displays help about the list of commands available.
 */
int cmd_help(int sockfd, char **args) {
  char msg[BUFSIZ];
  int i;

  memset(msg, 0, sizeof msg);
  if(args[0] == NULL || sockfd < 0)
    return -1;

  snprintf(msg, sizeof msg, "*** Help Below ***\r\n");
  for(i = 0; i < cmd_len(); i++) {
    strncat(msg, builtin_str[i], sizeof msg);
    strncat(msg, " - ", sizeof msg);
    strncat(msg, builtin_help[i], sizeof msg);
  }
  strncat(msg, "*** End Help ***\r\n", sizeof msg);
  if(send(sockfd, msg, strlen(msg), 0) != (int)strlen(msg))
    puts("Error: Could not send data to client.");
  
  return 1;
}

/* cmd_exit() - exits the remote shell.
 */
int cmd_exit(int sockfd, char **args) {
  if(args[0] == NULL || sockfd == 0)
    return 1;

  return 0;
}

/* cmd_split() - split an entire command string into tokens.
 */
char **cmd_split(char line[]) {
  char **tokens = NULL;
  char *token = NULL;
  int i = 0, size;

  size = CMD_TOK_SIZE * sizeof(char *);
  tokens = (char **)malloc(size);
  CHECK_MEM(tokens);

  token = strtok(line, CMD_TOK_DELIMS);
  while(token != NULL) {
    tokens[i] = token;
    ++i;
    if(i >= size) {
      size += CMD_TOK_SIZE * sizeof(char *);
      tokens = (char **)realloc(tokens, size);
      CHECK_MEM(tokens);
    }
    token = strtok(NULL, CMD_TOK_DELIMS);
  }
  tokens[i] = NULL;
  return tokens;
  
error:
  return NULL;
}

/* cmd_execute() - executes all builtin commands.
 */
int cmd_execute(int sockfd, char **args) {
  char data[BUFSIZ];
  int i;

  memset(data, 0, sizeof(data));
  if(args == NULL) {
    snprintf(data, sizeof(data), "Error: No arguments given.\r\n");
    if(send(sockfd, data, strlen(data), 0) < 0)
      puts("Error: Could not receive data from client.");
    return 1;
  } else if(args[0] == NULL) {
    snprintf(data, sizeof data, "Error: No arguments given.\r\n");
    if(send(sockfd, data, strlen(data), 0) < 0)
      puts("Error: Could not receive data from client.");
    return 1;
  }

  for(i = 0; i < cmd_len(); i++)
    if(strcmp(args[0], builtin_str[i]) == 0)
      return (*builtin_func[i])(sockfd, args);

  snprintf(data, sizeof data, "Error: Command not found.\r\n");
  if(send(sockfd, data, strlen(data), 0) < 0)
    puts("Error: Cannot send data to client.");
  return 1;
}

/* cmd_loop() - main command interface loop.
 */
void cmd_loop(int *sockfd, struct sockaddr_in *client) {
  FILE *sockin = NULL;
  char line[CMD_LEN];
  char msg[1024];
  char **args = NULL;
  int status;

  if(client == NULL)
    return;

  /* open file descriptors in file pointers */
  sockin = fdopen(*sockfd, "rb");
  if(sockin == NULL) {
    perror("fdopen");
    return;
  }
  if(setvbuf(sockin, NULL, _IOLBF, 0) != 0) {
    perror("setvbuf (read)");
    fclose(sockin);
    return;
  }

#ifdef __linux
  speakInit();
#endif
  
  do {
    memset(line, 0, sizeof line);
    memset(msg, 0, sizeof msg);
    snprintf(msg, sizeof msg, "CMD > ");
    if(send(*sockfd, msg, strlen(msg), 0) < 0)
      puts("Error: Cannot send message to client.");
    if(fgets(line, sizeof line, sockin) == NULL)
      puts("Error: Cannot recv from client.");
    args = cmd_split(line);
    status = cmd_execute(*sockfd, args);
    free(args);
  } while(status);
  fclose(sockin);
  
#ifdef __linux
  speakCleanup();
#endif
}
