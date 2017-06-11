#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int strlen_p(const char *str) {
	int i = 0;
	while(*(str+i) != 0) {
		i++;
	}
	return i;
}

int main() {
	const char *str = "String to encrypt.";
	int length = strlen_p(str);
	char *enc = malloc(length+1);
	char *dec = malloc(length+1);
	int i = 0;

	while(i < length) {
		*(enc+i) = *(str+i);
		*(enc+i) ^= ~(1 << 4) | 0xf0;
		*(dec+i) = *(enc+i);
		*(dec+i) ^= ~(1 << 4) | 0xf0;
		i++;
	}
	*(enc+i) = '\0';
	*(dec+i) = '\0';

	printf("Normal string: %s\n", str);
	printf("Encrypted string: %s\n", enc);
	printf("Decrypted string: %s\n", dec);

	free(enc);
	free(dec);
	return 0;
}
