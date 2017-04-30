#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


#include "objbase.h"


/* List of object functions the way they appear inside of objbase.h */

int Object_Startup(void)
{
	return 1;
}

void Object_InitFunc(void)
{
	printf("Default objbase init function...\n");
}

void Object_Destroy(void *self)
{
	object *obj = self;

	if (obj != NULL) {
		free(obj);
		obj = NULL;
	}
}

void *Object_new(size_t size, object proto)
{
	/* setup default functions in case they aren't set */
	if(!proto.Startup) proto.Startup = Object_Startup;
	if(!proto.InitFunc) proto.InitFunc = Object_InitFunc;
	if(!proto.Destroy) proto.Destroy = Object_Destroy;

	object *el = calloc(1, size);
	*el = proto;

	if(!el->Startup()) {
		/* looks like it didn't initialize properly */
		el->Destroy(el);
		return NULL;
	}
	else {
		/* all done */
		return el;
	}
}
