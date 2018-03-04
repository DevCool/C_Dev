#include "code16gcc.h"
__asm__("jmpl $0x0, $main;");

void putch(char ch)
{
	__asm__ __volatile__(
		"pusha;"
		"mov $0x0E, %%ah;"
		"mov %0, %%al;"
		"mov $0x0007, %%bx;"
		"mov $1, %%cx;"
		"int $0x10;"
		"popa;"
		:
		: "r"(ch)
	);
}

void print(const char *s)
{
	while (*s) {
		putch(*s++);
	}
}

char getch(void)
{
	char ch;
	__asm__ __volatile__(
		"mov $0x00, %%ah;"
		"mov %%al, %0;"
		"int $0x16;"
		: "=r"(ch)
		:
	);
	return ch;
}

void main(void)
{
	unsigned char ch, i;
	for (;;) {
		print("Press 'q' to reboot system...\r\n"
			"Press 'e' to wipe CMOS!\r\n");
		ch = getch();
		switch (ch) {
		case 'q':
		case 'Q':
			__asm__("jmp $0xFFFF, $0x0000;");
			break;
		case 'e':
		case 'E':
			print("Wiping CMOS...\r\n");
			for (i = 0; i < 255; i++)
				__asm__ __volatile__(
					"xor %ax, %ax;"
					"in $0x70, %ax;"
					"out %ax, $0x71;"
				);
			break;
		default:
			print("Invalid key pressed\r\n");
			break;
		}
	}
}

