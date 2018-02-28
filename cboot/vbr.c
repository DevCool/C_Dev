__asm__(".code16gcc\n");
__asm__("jmpl $0, $boot_main\n");

void putch(char ch)
{
	__asm__ __volatile__(
		"mov $0x0E, %%ah;"
		"mov %0, %%al;"
		"mov $0x000A, %%bx;"
		"int $0x10;"
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

/*
unsigned char getche(void)
{
	unsigned char ch;
	__asm__ __volatile__(
		"mov $0x00, %%ah;"
		"mov %%al, %0;"
		"int $0x16;"
		: "=r"(ch)
		:
	);
	__asm__ __volatile__(
		"mov $0x0E, %%ah;"
		"mov %%al, %0;"
		"mov $0x0A, %%bl;"
		"int $0x10;"
		:
		: "r"(ch)
	);
	return ch;
}
*/

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

void reboot(void)
{
	__asm__(
		"jmp $0xFFFF, $0x0000\n"
	);
}

void clear_cmos(void)
{
	unsigned char i = 0;
	while (i++ <= 255) {
		__asm__ __volatile__(
			"xor %ax, %ax;"
			"in $70, %ax;"
			"out %ax, $71;"
		);
	}
}

void init_graphics(void)
{
	__asm__ __volatile__(
		"mov $0x0003, %ax;"
		"int $0x10;"
		"mov $0x0013, %ax;"
		"int $0x10;"
	);
}

void boot_main(void)
{
	char ch;
	init_graphics();
	while ((ch = getch()) != 'q') {
		if (ch == 'e')
			clear_cmos();
		else
			print("Press 'q' to reboot...\r\n");
	}
	reboot();
}
