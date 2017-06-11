#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "object.h"

void Object_destroy(void *self)
{
	Object *obj = self;

	if(obj) {
		if(obj->description) free(obj->description);
		free(obj);
	}
}

void Object_describe(void *self)
{
	Object *obj = self;
	printf("%s.\n", obj->description);
}

int Object_init(void *self)
{
	/* do nothing really */
	return 1;
}

void *Object_move(void *self, Direction direction)
{
	printf("You can't go that direction.\n");
	return NULL;
}

int Object_attack(void *self, int damage)
{
	printf("You can't attack that.\n");
	return 0;
}

void *Object_new(size_t size, Object proto, char *description)
{
	/* setup the default functions in case they aren't set */
	if(!proto.init) proto.init = Object_init;
	if(!proto.describe) proto.describe = Object_describe;
	if(!proto.destroy) proto.destroy = Object_destroy;
	if(!proto.attack) proto.attack = Object_attack;
	if(!proto.move) proto.move = Object_move;

	/* this seems weird, but I had to make a struct and
	 * point a different pointer to it to cast it */
	Object *el = calloc(1, size);
	*el = proto;

	el->description = strdup2(description);

	/* initialize it with whatever init we were given */
	if(!el->init(el)) {
		/* looks like it didn't initialize properly */
		el->destroy(el);
		return NULL;
	} else {
		/* all done, successfully have an object */
		return el;
	}
}

char *strdup2(const char *str)
{
	char *result = (char*)malloc(strlen(str)+1);
	char *pdst = result;
	if(!result && !str) return NULL;
	while(*str != 0) {
		*pdst++ = *str++;
	}
	*pdst = '\0';
	return result;
}
