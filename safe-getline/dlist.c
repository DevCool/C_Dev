/*************************************************************
 * dlist.c - this is my simple singly linked list code.
 * by 5n4k3
 *************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* external function prototypes */
extern void pzero(char* s, size_t size);

/* define DLIST structures */
typedef struct _DLIST_struct DLIST;
struct _DLIST_struct {
	char data[BUFSIZ];
	unsigned char set;
	DLIST *next;
};

/* ----------- functions below are only for DLIST structs ---------- */

DLIST* DLIST_create(void) {
	DLIST *obj;
	obj = (DLIST*)malloc(sizeof(DLIST));
	assert(obj != NULL);
	memset(obj->data, 0, sizeof(obj->data));
	obj->set = 0;
	obj->next = NULL;
	return obj;
}

DLIST* DLIST_add(DLIST *list, char *data, unsigned char set) {
	DLIST *tmp;
	tmp = DLIST_create();
	assert(tmp != NULL);
	strncpy(tmp->data, data, sizeof(tmp->data));
	tmp->set = set;
	if(list == NULL) {
		list = tmp;
	} else {
		tmp->next = list;
		list = tmp;
	}
	return list;
}

void DLIST_destroy(DLIST *list) {
	DLIST *tmp;
	assert(list != NULL);
	tmp = list;
	while(tmp != NULL) {
		list = list->next;
		free(tmp);
		tmp = list;
	}
}

void DLIST_print(DLIST *list) {
	DLIST *tmp = NULL;
	int i;

	assert(list != NULL);
	i = 0;
	tmp = list;
	while(tmp != NULL) {
		printf("Item %d : DATA [%s]\n", ++i, tmp->data);
		tmp = tmp->next;
	}
}

/* ----------------- End of my DLIST functions ------------------ */

void test3(void) {
	char *data[] = {"Some data 1", "Some data 2", "Some data 3"};
	DLIST *list = NULL;
	int i;

	for(i = 0; i < 3; i++)
		if((list = DLIST_add(list, data[i], 1)) == NULL) {
			puts("Error: Couldn't create node for list.");
			DLIST_destroy(list);
		}
	puts(" *** Start of Test 3 ***");
	DLIST_print(list);
	DLIST_destroy(list);
	puts("  *** End of test 3 ***");
}

