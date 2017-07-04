/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DLIST include */
#include "dlist.h"

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
  node->next = NULL;
  node->number = 0;
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
void DList_free(DList **head) {
  DList *cur = NULL;
  DList *tmp = NULL;
  cur = *head;
  while(cur != NULL) {
    tmp = cur;
    cur = cur->next;
    destroy_node(tmp);
  }
  *head = NULL;
}

void DList_freebeg(DList **head) {
  DList *next = NULL;
  if(*head == NULL)
    return;
  next = (*head)->next;
  destroy_node(*head);
  *head = next;
}

void DList_freelast(DList **head) {
  DList *cur = NULL;
  if(*head == NULL)
    return;
  
  if((*head)->next == NULL) {
    DList_freebeg(head);
    *head = NULL;
  } else {
    cur = *head;
    while(cur->next->next != NULL) {
      cur = cur->next;
    }
    destroy_node(cur->next);
    cur->next = NULL;
  }
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
