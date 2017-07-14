#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "socket.h"
#include "../../debug.h"
#include "helper.h"

char *builtin_str[] = {
  "help",
  "exit"
};

int cmd_help(int sockfd, char **args) {
  char msg[BUFSIZ];

  memset(msg, 0, sizeof(msg));
  if(args[0] == NULL) {
    snprintf(msg, sizeof msg, "No commands except 'exit' available.\r\n");
    if(send(sockfd, msg, strlen(msg), 0) < 0)
      puts("Error: Could not send data to client.");
  }

  snprintf(msg, sizeof msg, "Sorry, 'exit' is the only command right now.\r\n");
  if(send(sockfd, msg, strlen(msg), 0) < 0)
    puts("Error: Could not send data to client.");
  
  return 1;
}

int cmd_exit(int sockfd, char **args) {
  if(args[0] == NULL || sockfd == 0)
    return 1;

  return 0;
}

int (*builtin_func[])(int sockfd, char **args) = {
  &cmd_help,
  &cmd_exit
};

int cmd_len(void) {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
char *cmd_getln(int sockfd) {
  char *line = NULL;
  int size, c, i;

  size = CMD_LEN;
  line = realloc(line, size);
  CHECK_MEM(line);
  
  while(1) {
    c = getc(sockfd);
    if(c == EOF || c == '\r' || c == '\n') {
      line[i] = 0;
      return line;
    } else {
      line[i] = c;
    }
    ++i;
    if(i >= size) {
      size += CMD_LEN;
      line = realloc(line, size);
      CHECK_MEM(line);
    }
  }

error:
  return NULL;
}
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

void cmd_loop(int *sockfd) {
  char line[CMD_LEN];
  char msg[1024];
  char **args = NULL;
  int status;

  do {
    memset(line, 0, sizeof line);
    memset(msg, 0, sizeof msg);
    snprintf(msg, sizeof msg, "CMD > ");
    if(send(*sockfd, msg, strlen(msg), 0) < 0)
      puts("Error: Cannot send message to client.");
    if(recv(*sockfd, line, sizeof line, 0) < 0)
      puts("Error: Cannot recv from client.");
    args = cmd_split(line);
    status = cmd_execute(*sockfd, args);
    free(args);
  } while(status);
}
