/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* DLIST */
#include "dlist.h"
#include "cmd.h"

/* function prototypes */
#if !defined(_WIN32) || (_WIN64)
int stricmp(const char *s, const char *s2);
#endif
int get_line(char *s, int size);
void DList_save(DList *head);
DList *DList_load(DList *list);

/* main() - entry point for this text editor.
 */
int main(void) {
  DList *list = NULL;
  unsigned char is_created = 0, cmd = CMD_COUNT;
  int cnt = 1;

  printf("TexEd v0.02 by Philip R. Simonson\n******\n\n");
  do {
    if(list == NULL) {
      is_created = 0;
      cnt = 1;
    }
    cmd = get_cmd();
    list = process_cmds(cmd, list, &cnt, &is_created);
  } while(cmd != CMD_EXIT);

  if(is_created)
    DList_free(&list);
  return 0;
}

#if !defined(_WIN32) || (_WIN64)
/* stricmp() - incase sensitive string compare for *nix.
 */
int stricmp(const char *s, const char *s2) {
  int i;
  
  for(i = 0; s < s+1 && *s != 0; i++) {
    if(tolower(*s) != tolower(*s2))
      return (tolower(*s)-tolower(*s2));
    s++;
    s2++;
  }
  return 0;
}
#endif

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
      DList_addnode(list, data, ++cnt);
    }
    memset(buf, 0, sizeof(buf));
    memset(data, 0, sizeof(data));
  }
  fclose(file);
  puts("File loaded successfully!");
  return list;
}
