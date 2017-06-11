#ifndef terminal_h
#define terminal_h

#if !defined(pstr_h)
#include "pstr.h"
#endif

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_clr;
uint16_t* terminal_buf;

void terminal_initialize() {
	size_t x, y;
	terminal_row = 0;
	terminal_col = 0;
	terminal_clr = make_color(COLOR_LIGHT_GREY, COLOR_DARK_GREY);
	terminal_buf = (uint16_t*) 0xB8000;
	for (y = 0; y < VGA_HEIGHT; y++) {
		for (x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buf[index] = make_vgaentry(' ', terminal_clr);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_clr = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buf[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c) {
	terminal_putentryat(c, terminal_clr, terminal_col, terminal_row);
	if (++terminal_col == VGA_WIDTH) {
		terminal_col = 0;
		if (++terminal_row == VGA_HEIGHT) {
			terminal_row = 0;
		}
	}
}

void terminal_writestring(const char *data) {
	size_t datalen = strlen(data);
	size_t i;
	for (i = 0; i < datalen; i++) {
		if (data[i] == '\n') {
			terminal_row++;
			terminal_col = 0;
		} else {
			terminal_putchar(data[i]);
		}
	}
}

#endif
