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
			"int $0x10" : : "a"(0x0E00 | *s), "b"(0x0007)
		);
		s++;
	}
}

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

void reboot(void)
{
	__asm__(
		"jmp $0xFFFF, $0x0000\n"
	);
}

void init_graphics(void)
{
	__asm__(
		"mov $0x0003, %ax;"
		"int $0x10;"
		"mov $0x0013, %ax;"
		"int $0x10;"
	);
}

void boot_main(void)
{
	print("Press 'q' to reboot... any key to continue.\r\n");
	getche();
	init_graphics();
	while (getche() != 'q');
	reboot();
}
