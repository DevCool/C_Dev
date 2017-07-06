/* standard headers */
#include <stdio.h>
#include <string.h>

/* My headers */
#include "dlist.h"
#include "cmd.h"

/* defines */
#define MAX_LINE 80

/* function prototypes for processing the list and commands */
#if !defined(_WIN32) || (_WIN64)
extern int stricmp(const char *s, const char *s2);
#endif
extern int get_line(char *s, size_t size);
extern void DList_save(DList *head);
extern DList *DList_load(DList *head);

static char *cmds[] = {
  "", "new", "add", "addbeg", "free",
  "del", "print", "help", "save",
  "load", "exit"
};

/* get_cmd() - gets a command then sends to process_cmds().
 */
unsigned char get_cmd(void) {
  char buf[BUFSIZ];

  memset(buf, 0, sizeof(buf));
  printf("CMD >> ");
  get_line(buf, sizeof(buf));
  if(stricmp(buf, cmds[CMD_NEW]) == 0) {
    return CMD_NEW;
  } else if(stricmp(buf, cmds[CMD_ADD]) == 0) {
    return CMD_ADD;
  } else if(stricmp(buf, cmds[CMD_ADDBEG]) == 0) {
    return CMD_ADDBEG;
  } else if(stricmp(buf, cmds[CMD_FREE]) == 0) {
    return CMD_FREE;
  } else if(stricmp(buf, cmds[CMD_DEL]) == 0) {
    return CMD_DEL;
  } else if(stricmp(buf, cmds[CMD_PRINT]) == 0) {
    return CMD_PRINT;
  } else if(stricmp(buf, cmds[CMD_HELP]) == 0) {
    return CMD_HELP;
  } else if(stricmp(buf, cmds[CMD_SAVE]) == 0) {
    return CMD_SAVE;
  } else if(stricmp(buf, cmds[CMD_LOAD]) == 0) {
    return CMD_LOAD;
  } else if(stricmp(buf, cmds[CMD_EXIT]) == 0) {
    return CMD_EXIT;
  } else if(stricmp(buf, cmds[CMD_NONE]) == 0) {
    return CMD_NONE;
  }
  return CMD_COUNT;
}

/* process_cmds() - as the name suggests, processes all commands entered.
 */
DList *process_cmds(unsigned char cmds, DList *list, int *cnt, unsigned char *is_created) {
  switch(cmds) {
    
  case CMD_NONE:
    printf("Command not found, please enter 'help' to see available commands.\n");
    break;
    
  case CMD_NEW:
    if(!*is_created) {
      char data[MAX_LINE];
      memset(data, 0, sizeof(data));
      puts("  *** Enter Text Below ***");
      get_line(data, sizeof(data));
      list = create_node();
      set_node(list, data, (*cnt)++);
      *is_created = 1;
      puts("List created.");
    } else {
      puts("List already exists.");
    }
    break;

  case CMD_ADD:
    if(*is_created) {
      char data[MAX_LINE];
      memset(data, 0, sizeof(data));
      puts("  *** Enter Text Below ***");
      get_line(data, sizeof(data));
      DList_addnode(list, data, (*cnt)++);
      puts("Data added.");
    } else {
      puts("List does not exist.");
    }
    break;

  case CMD_ADDBEG:
    if(*is_created) {
      char data[MAX_LINE];
      memset(data, 0, sizeof(data));
      puts("  *** Enter Text Below ***");
      get_line(data, sizeof(data));
      DList_addbeg(&list, data, (*cnt)++);
      DList_recalculate(list);
      puts("Data added.");
    } else {
      puts("List does not exist.");
    }
    break;

  case CMD_FREE:
    if(*is_created) {
      DList_free(&list);
      puts("List destroyed.");
      *is_created = 0;
      *cnt = 0;
    } else {
      puts("List does not exist.");
    }
    break;

  case CMD_DEL:
    if(*is_created) {
      DList_free(&list);
      (*cnt)--;
      puts("Destroyed last element.");
    } else {
      puts("List does not exist.");
    }
    break;

  case CMD_PRINT:
    if(*is_created) {
      puts("  *** Displaying list ***");
      DList_print(list);
      puts("  *** End of list ***");
    } else {
      puts("List does not exist.");
    }
    break;
    
  case CMD_HELP:
    printf("    *** List of available commands ***\n"\
	   "******************************************\n"\
	   "new    - create the initial list\n"\
	   "add    - add a new node to end of list\n"\
	   "addbeg - add a new node to beg of list\n"\
	   "print  - prints the entire list\n"\
	   "del    - deletes last element\n"\
	   "free   - destroys the entire list\n"\
	   "save   - saves to a file\n"\
	   "load   - loads from a file\n"\
	   "help   - prints available commands\n"\
	   "exit   - quits the program\n"\
	   "******************************************\n"\
	   "      *** End of the command list ***\n");
    break;

  case CMD_SAVE:
    if(*is_created) {
      DList_save(list);
    } else {
      puts("List does not exist.");
    }
    break;

  case CMD_LOAD:
    if(!*is_created) {
      if((list = DList_load(list)) == NULL) {
	puts("Failed to load list.");
      } else {
	*is_created = 1;
      }
    } else {
      puts("List already exists.");
    }
    break;

  case CMD_EXIT:
    break;

  default:
    puts("Unknown command.");
    
  }
  
  return list;
}
