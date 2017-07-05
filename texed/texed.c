/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* DLIST */
#include "dlist.h"

/* defines */
#define MAX_LINE 80

/* function prototypes */
int strnicmp(const char *s, const char *s2, size_t size);
int get_line(char *s, int size);
void DList_save(DList *head);
DList *DList_load(DList *list);

/* main() - entry point for this text editor.
 */
int main(void) {
  char buf[MAX_LINE];
  DList *list = NULL;
  unsigned char is_created;
  int buf_len, cnt;

  do {
    if(list == NULL) {
      is_created = 0;
      cnt = 1;
    }
    memset(buf, 0, sizeof(buf));
    printf("CMD >> ");
    buf_len = get_line(buf, MAX_LINE);
    if(strnicmp(buf, "new", 3) == 0) {
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
	puts("List already exists.");
      }
    } else if(strnicmp(buf, "addbeg", 6) == 0) {
      if(is_created) {
	char data[MAX_LINE];
	memset(data, 0, sizeof(data));
	printf("*** Enter text below ***\n");
	get_line(data, sizeof(data));
	DList_addbeg(&list, data, cnt++);
	DList_recalculate(list);
	puts("Data added.");
      } else {
	puts("List does not exist.");
      }
    } else if(strnicmp(buf, "add", 3) == 0) {
      if(is_created) {
	char data[MAX_LINE];
	memset(data, 0, sizeof(data));
	printf("*** Enter text below ***\n");
	get_line(data, sizeof(data));
	DList_addnode(&list, data, cnt++);
	puts("Data added.");
      } else {
	puts("List does not exist.");
      }
    } else if(strnicmp(buf, "del", 3) == 0) {
      if(is_created) {
	DList_freelast(&list);
	puts("Destroyed last element.");
      } else {
	puts("List alread freed!");
      }
    } else if(strnicmp(buf, "print", 5) == 0
	      || strnicmp(buf, "list", 4) == 0) {
      if(is_created) {
	DList_print(list);
	puts("  *** End of list ***");
      } else {
	puts("List does not exist.");
      }
    } else if(strnicmp(buf, "free", 4) == 0) {
      if(is_created) {
        DList_free(&list);
        puts("List freed!");
        is_created = 0;
      } else {
	puts("List does not exist!");
      }
    } else if(strnicmp(buf, "save", 4) == 0) {
      if(is_created) {
	DList_save(list);
      } else {
	puts("List does not exist.");
      }
    } else if(strnicmp(buf, "load", 4) == 0) {
      if(!is_created) {
	if((list = DList_load(list)) == NULL) {
	  puts("Failed to load file.");
	} else {
	  is_created = 1;
	}
      } else {
	puts("List already exists.");
      }
    } else if(strnicmp(buf, "help", 4) == 0) {
      printf(" *** HELP ***\n"\
	     " new    - create the initial list\n"\
	     " help   - prints this message\n"\
	     " add    - add new node to list\n"\
	     " addbeg - add new node to beginning\n"\
	     " print  - prints the entire list\n"\
	     " del    - removes last entry in list\n"\
	     " save   - saves to a file\n"\
	     " load   - loads a file\n"\
	     " free   - frees the entire list\n"\
	     " exit   - quits this program\n"\
	     "************************************\nEnd of commands.\n\n");
    } else if(strnicmp(buf, "exit", 4) == 0) {
      break;
    } else {
      puts("Command not found.");
    }
  } while(buf_len > 0);

  if(is_created)
    DList_free(&list);
  return 0;
}

/* stricmp() - incase sensitive string compare for *nix.
 */
int strnicmp(const char *s, const char *s2, size_t size) {
  size_t i;
  
  for(i = 0; *s != 0 && i < size; i++) {
    if(tolower(*s) != tolower(*s2))
      return (tolower(*s)-tolower(*s2));
    s++;
    s2++;
  }
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
  /*
  if(c == 0x0A) {
    s[i] = c;
    ++i;
    ++j;
  }
  */
  s[j] = 0;
  ++j;
  return i;
}

/* DList_save() - saves the linked list to a file.
 */
void DList_save(DList *head) {
  char name[BUFSIZ];
  int len;
  DList *cur = NULL;
  FILE *file = NULL;

  if(head == NULL)
    return;

  memset(name, 0, sizeof(name));
  printf("Enter a filename: ");
  if(!((len = get_line(name, sizeof(name))) > 0)) {
    puts("Cannot create file without a name.");
    return;
  }
  if((file = fopen(name, "wt")) == NULL) {
    fprintf(stderr,
	   "Error: Cannot create file.\n%s is an invalid filename.\n",
	   name);
    return;
  }
  cur = head;
  while(cur != NULL) {
    fprintf(file, "%d : %s\n", cur->number, cur->description);
    cur = cur->next;
  }
  fclose(file);
  puts("File written successfully.");
}

DList *DList_load(DList *list) {
  char name[BUFSIZ];
  char buf[BUFSIZ];
  char data[BUFSIZ];
  int cnt = 0, len;
  FILE *file = NULL;

  memset(name, 0, sizeof(name));
  printf("Enter a filename: ");
  if(!((len = get_line(name, sizeof(name))) > 0)) {
    puts("Cannot create file without a name.");
    return NULL;
  }
  if((file = fopen(name, "rt")) == NULL) {
    fprintf(stderr,
	    "Error: Cannot open file.\n%s is an invalid format.\n",
	    name);
    return NULL;
  }
  memset(buf, 0, sizeof(buf));
  memset(data, 0, sizeof(data));
  while(fgets(buf, sizeof(buf), file) != NULL) {
    sscanf(buf, "%*d : %[^\n]\n", data);
    if(list == NULL) {
      list = create_node();
      set_node(list, data, ++cnt);
    } else {
      DList_addnode(&list, data, ++cnt);
    }
    memset(buf, 0, sizeof(buf));
    memset(data, 0, sizeof(data));
  }
  fclose(file);
  puts("File loaded successfully!");
  return list;
}
