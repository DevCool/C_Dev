#ifndef pstr_h
#define pstr_h

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str) {
	size_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
}

#endif
