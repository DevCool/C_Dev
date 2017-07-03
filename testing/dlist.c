/* Standard headers */
#include <stdio.h>
#include <stdlib.h>

/* DLIST Crap */
struct DLIST {
  unsigned char ch;
  struct DLIST *next;
};

typedef struct DLIST DList;

/* DLIST node functions */
DList *create_node(void);
void destroy_node(DList *node);
void set_node(DList *node, unsigned char ch);

/* DLIST functions */
void DList_free(DList *head);
void DList_freelast(DList *head);
void DList_print(DList *head);
void DList_setlast(DList *head, unsigned char ch);
void DList_addnode(DList *head, unsigned char ch);


/* main() - entry point of program.
 */
int main(void) {
  DList *head = create_node();

  set_node(head, 'a');
  DList_addnode(head, 'b');
  DList_print(head);
  DList_free(head);
  
  return 0;
}


/* DLIST Node functions */
DList *create_node(void) {
  DList *ret = malloc(sizeof(DList));
  if(ret == NULL)
    return NULL;
  ret->ch = 0;
  ret->next = NULL;
  return ret;
}

void destroy_node(DList *node) {
  if(node == NULL)
    return;
  free(node);
}

void set_node(DList *node, unsigned char ch) {
  if(node == NULL)
    return;
  node->ch = ch;
}

  
/* DLIST functions */
void DList_free(DList *head) {
  DList *cur = NULL;
  DList *tmp = NULL;
  cur = head;
  while(cur != NULL) {
    tmp = cur;
    cur = cur->next;
    free(tmp);
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
    printf("%c", tmp->ch);
    tmp = tmp->next;
  }
  putchar('\n');
}

void DList_setlast(DList *head, unsigned char ch) {
  DList *tmp = NULL;
  tmp = head;
  while(tmp->next->next != NULL) {
    tmp = tmp->next;
  }
  set_node(tmp->next, ch);
}

void DList_addnode(DList *head, unsigned char ch) {
  DList *current = head;
  while(current->next != NULL) {
    current = current->next;
  }
  current->next = create_node();
  DList_setlast(head, ch);
}
