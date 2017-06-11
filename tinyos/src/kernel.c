#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#include "vga.h"
#include "pstr.h"
#include "terminal.h"

#if defined(__cplusplus)
extern "C"
#endif
void p_main() {
	terminal_initialize();

	terminal_writestring("Hello, testing from my simple kernel.\n\n\n"
		"       --ThePlague1988\n");
}
