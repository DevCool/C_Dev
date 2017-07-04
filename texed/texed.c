/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DLIST */
#include "dlist.h"

/* defines */
#define MAX_LINE 80

/* function prototypes */
int get_line(char *s, int size);

/* main() - entry point for this text editor.
 */
int main(int argc, char *argv[]) {
  char buf[MAX_LINE];
  DList *list = NULL;
  unsigned char is_created;
  int buf_len, cnt = 1;

  do {
    memset(buf, 0, sizeof(buf));
    printf("CMD >> ");
    buf_len = get_line(buf, MAX_LINE);
    if(strncmp(buf, "new", 3) == 0) {
      if(!is_created) {
      char data[MAX_LINE];
      list = create_node();
      memset(data, 0, sizeof(data));
      printf("*** Enter text below ***\n");
      get_line(data, sizeof(data));
      set_node(list, data, cnt++);
      is_created = 1;
      puts("List created.");
      } else {
	puts("List already created.");
      }
    } else if(strncmp(buf, "add", 3) == 0) {
      if(is_created) {
	char data[MAX_LINE];
	memset(data, 0, sizeof(data));
	printf("*** Enter text below ***\n");
	get_line(data, sizeof(data));
	DList_addnode(list, data, cnt++);
	puts("Data added.");
      } else {
	puts("List not yet created.");
      }
    } else if(strncmp(buf, "del", 3) == 0) {
      puts("Not yet implemented.");
      /*
      if(is_created) {
	if(list != NULL) {
	  DList_freelast(list);
	  puts("Destroyed last element.");
	}
	if(list == NULL) {
	  is_created = 0;
	}
      } else {
	puts("List alread freed!");
      }
      */
    } else if(strncmp(buf, "print", 4) == 0) {
      if(is_created) {
	DList_print(list);
	puts("  *** End of list ***");
      } else {
	puts("List doesn't exist.");
      }
    } else if(strncmp(buf, "free", 4) == 0) {
      if(is_created) {
        DList_free(list);
        puts("List freed!");
        is_created = 0;
      } else {
	puts("List not created!");
      }
    } else if(strncmp(buf, "help", 4) == 0) {
      printf(" *** HELP ***\n"\
	     " new   - create the initial list\n"\
	     " help  - prints this message\n"\
	     " add   - add new node to list\n"\
	     " print - prints the entire list\n"\
	     " del   - removes last entry in list\n"\
	     " free  - frees the entire list\n"\
	     " exit  - quits this program\n"\
	     "************************************\nEnd of commands.\n\n");
    } else if(strncmp(buf, "exit", 4) == 0) {
      break;
    } else {
      puts("Command not found.");
    }
  } while(buf_len > 0);

  if(is_created)
    DList_free(list);
  return 0;
}

/* get_line() - gets a string from the standard input.
 */
int get_line(char *s, int size) {
  int c, i, j;

  j = 0;
  for(i = 0; (c = getchar()) != EOF && c != 0x0A; i++)
    if(!(i < 0) && i < size-2) {
      s[i] = c;
      ++j;
    }
  if(c == 0x0A) {
    s[i] = c;
    ++i;
    ++j;
  }
  s[j] = 0;
  ++j;
  return i;
}
