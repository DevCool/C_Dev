int main()
{
	unsigned int *vidmem=(unsigned int*)0xb8000;
	char c = 'H';
	vidmem=(unsigned int*)(c|0x00f0);
	return 0;
}