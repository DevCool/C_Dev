__asm__(".code16gcc\n");
__asm__("jmpl $0, $boot_main\n");

unsigned short str_len(const char *s)
{
	unsigned short i;
	for (i=0; *(s+i) != '\0'; ++i);
	return i;
}

void print(const char *s)
{
	while (*s) {
		__asm__ __volatile__(
			"int $0x10"
			:
			: "a"(0x0E00 | *s), "b"(0x0005)
		);
		s++;
	}
}

unsigned char getch(void)
{
	unsigned char ch;
	__asm__ __volatile__(
		"mov $0x00, %%ah\n\t"
		"mov %%al, %0\n\t"
		"int $0x16"
		: "=r"(ch)
		:
	);
	return ch;
}

unsigned char getche(void)
{
	unsigned char ch;
	ch = getch();
	__asm__(
		"mov $0x0E, %%ah;"
		"mov %%al, %0;"
		"int $0x10;"
		:
		: "r"(ch)
	);
	return ch;
}

void reboot(void)
{
	__asm__ __volatile__(
		"jmp $0xFFFF, $0x0000\n"
	);
}

void boot_main(void)
{
	print("Test Boot Sector by Philip Simonson\r\n"
		"\r\nPress 'q' to reboot...\r\n");
	while (getche() != 'q');
	reboot();
}
