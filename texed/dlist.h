#ifndef _dlist_h_
#define _dlist_h_


/* DLIST structure */
struct DLIST {
  char *description;
  int number;
  struct DLIST *next;
};

typedef struct DLIST DList;


/* DLIST node function prototypes */
DList *create_node(void);
void destroy_node(DList *node);
void set_node(DList *node, const char *string, int item);

/* DList function prototypes */
void DList_free(DList **head);
void DList_freebeg(DList **head);
void DList_freelast(DList **head);
void DList_print(DList *head);
void DList_setlast(DList *head, const char *string, int item);
void DList_addnode(DList *head, const char *string, int item);
void DList_addbeg(DList **head, const char *string, int item);
void DList_recalculate(DList *head);


#endif
