#ifndef helper_h
#define helper_h

#define CMD_LEN 256
#define CMD_TOK_SIZE 64
#define CMD_TOK_DELIMS " \r\n\t\a"

enum _COMMAND {
  CMD_NONE = -1,
  CMD_HELP,
  CMD_EXIT,
  CMD_COUNT
};

int cmd_len(void);
char *split_line(char *line);
void cmd_loop(int *sockfd, struct sockaddr_in *client);

#endif
