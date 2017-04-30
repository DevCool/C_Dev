#ifndef objbase_h
#define objbase_h

typedef struct object object;

struct object {
	int (*Startup)(void);
	void (*InitFunc)(void);
	void (*Destroy)(void *self);
};

/* prototypes for object functions */
int Object_Startup(void);
void Object_InitFunc(void);
void Object_Destroy(void *self);
void *Object_new(size_t size, object proto);

/* custom object defines, makes life easier in C */
#define NEW(T) Object_new(sizeof(T), T##Proto)
#define _(N) proto.N

#endif