#ifndef _cmd_h_
#define _cmd_h_

enum COMMANDS {
  CMD_NEW,
  CMD_ADD,
  CMD_ADDBEG,
  CMD_FREE,
  CMD_DEL,
  CMD_PRINT,
  CMD_HELP,
  CMD_SAVE,
  CMD_LOAD,
  CMD_EXIT,
  CMD_NONE,
  CMD_COUNT
};

unsigned char get_cmd(void);
DList *process_cmds(unsigned char cmds, DList *list, int *cnt, unsigned char *is_created);

#endif
