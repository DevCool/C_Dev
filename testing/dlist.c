/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DLIST Crap */
struct DLIST {
  char *description;
  int number;
  struct DLIST *next;
};

typedef struct DLIST DList;

/* DLIST node functions */
DList *create_node(void);
void destroy_node(DList *node);
void set_node(DList *node, const char *string, int item);

/* DLIST functions */
void DList_free(DList *head);
void DList_freelast(DList *head);
void DList_print(DList *head);
void DList_setlast(DList *head, const char *string, int item);
void DList_addnode(DList *head, const char *string, int item);


/* main() - entry point of program.
 */
int main(void) {
  int count = 1;
  DList *head = create_node(); /* create first node */

  /* Start message */
  printf("     *************************************\n"\
	 "     *** Dynamic Linked List - Example ***\n"\
	 "     *************************************\n\n");
  
  set_node(head, "Philip", count++); /* set first node data */
  DList_addnode(head, "Ruben", count++); /* add second node */
  DList_addnode(head, "Simonson", count++); /* add third node */
  DList_addnode(head, "Philip Ruben Simonson", count++); /* add fourth node */
  DList_print(head); /* prints the entire list */
  DList_free(head); /* frees the whole list */

  /* return success */
  return 0;
}


/* -------------- DLIST Node functions --------------- */
DList *create_node(void) {
  DList *ret = malloc(sizeof(DList));
  if(ret == NULL)
    return NULL;
  ret->description = NULL;
  ret->number = 0;
  ret->next = NULL;
  return ret;
}

void destroy_node(DList *node) {
  if(node == NULL)
    return;
  free(node->description);
  node->description = NULL;
  free(node);
  node = NULL;
}

void set_node(DList *node, const char *string, int item) {
  if(node == NULL)
    return;
  node->description = malloc(strlen(string)+1);
  strncpy(node->description, string, strlen(string)+1);
  node->number = item;
}

  
/* ------------------ DLIST functions ----------------- */
void DList_free(DList *head) {
  DList *cur = NULL;
  DList *tmp = NULL;
  cur = head;
  while(cur != NULL) {
    tmp = cur;
    cur = cur->next;
    destroy_node(tmp);
  }
}

void DList_freelast(DList *head) {
  DList *last = NULL;
  if(head->next == NULL) {
    free(head);
    return;
  }
  
  last = head;
  while(last->next->next != NULL) {
      last = last->next;
  }
  destroy_node(last->next);
  last->next = NULL;
}

void DList_print(DList *head) {
  DList *tmp = NULL;
  tmp = head;
  while(tmp != NULL) {
    printf("%d : %s\n", tmp->number, tmp->description);
    tmp = tmp->next;
  }
  putchar('\n');
}

void DList_setlast(DList *head, const char *string, int item) {
  DList *tmp = NULL;
  tmp = head;
  while(tmp->next->next != NULL) {
    tmp = tmp->next;
  }
  set_node(tmp->next, string, item);
}

void DList_addnode(DList *head, const char *string, int item) {
  DList *current = head;
  while(current->next != NULL) {
    current = current->next;
  }
  current->next = create_node();
  DList_setlast(head, string, item);
}
