__asm__(".code16gcc\n");
__asm__("jmpl $0, $boot_main\n");

void print(const char *s)
{
	while (*s) {
		__asm__ __volatile__(
			"int $0x10"
			:
			: "a"(0x0E00 | *s), "b"(7)
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

void reboot(void)
{
	__asm__ __volatile__(
		"jmp $0xFFFF, $0x0000\n"
	);
}

void boot_main(void)
{
	while(getch() != 'q') {
		print("Press 'q' to reboot...\r\n");
	}
	reboot();
}
