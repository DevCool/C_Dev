#ifndef DLLInjector_h
#define DLLInjector_h

#include <stdio.h>
#include <stdlib.h>

#include "objbase.h"

struct InitFunc {
	object proto;
};
__declspec(dllexport) typedef struct InitFunc InitFunc;

void PRS_Default(void) {
	printf("DLL: PRS_Default function is active, no function override detected.\n");
}

__declspec(dllexport) InitFunc *initFunc = NULL;

#endif
