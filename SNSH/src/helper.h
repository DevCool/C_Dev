#ifndef helper_h
#define helper_h

#define CMD_LEN 256
#define CMD_TOK_SIZE 64
#define CMD_TOK_DELIMS " \r\n\t\a"

/* commands enum */
enum _COMMAND {
  CMD_NONE = -1,
  CMD_CD,
  CMD_LS,
  CMD_RM,
  CMD_MKDIR,
  CMD_RMDIR,
  CMD_TOUCH,
  CMD_TYPE,
  CMD_WRITE,
  CMD_HOSTUP,
  CMD_TRANSFER,
#ifdef __linux__
  CMD_SPEAK,
  CMD_TERM,
  CMD_PIVOT,
#endif
  CMD_HELP,
  CMD_EXIT,
  CMD_COUNT
};

/* command function prototypes */
int cmd_cd(int sockfd, char **args);
int cmd_ls(int sockfd, char **args);
int cmd_rm(int sockfd, char **args);
int cmd_mkdir(int sockfd, char **args);
int cmd_rmdir(int sockfd, char **args);
int cmd_touch(int sockfd, char **args);
int cmd_type(int sockfd, char **args);
int cmd_write(int sockfd, char **args);
int cmd_hostup(int sockfd, char **args);
int cmd_transfer(int sockfd, char **args);
#ifdef __linux__
int cmd_speak(int sockfd, char **args);
int cmd_term(int sockfd, char **args);
int cmd_pivot(int sockfd, char **args);
#endif
int cmd_help(int sockfd, char **args);
int cmd_exit(int sockfd, char **args);

/* command handling function prototypes */
int cmd_len(void);
char *split_line(char *line);
void cmd_loop(int *sockfd, struct sockaddr_in *client);

#endif
