#include <stdio.h>

union BITFIELD1 {
	char a;
	int character;
};

int main()
{
	union BITFIELD1 bt1;

	bt1.a = 0x50;
	putchar(bt1.character);

	bt1.a = 0x52;
	putchar(bt1.character);

	bt1.a = 0x53;
	putchar(bt1.character);

	bt1.a = 0x0D;
	putchar(bt1.character);

	return 0;
}

