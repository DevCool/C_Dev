#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define SECRET_CODE "n038fyh38!^*$&"

int strlen_p(const char *str) {
	int i = 0;
	while(*(str+i) != 0) {
		i++;
	}
	return i;
}

int main() {
	const char *str = "String to encrypt.";
	const char *key = SECRET_CODE;
	int length = strlen_p(str);
	char *enc = malloc(length+1);
	char *dec = malloc(length+1);
	int i = 0;
	int j = 0;

	while(i < length) {
		*(enc+i) = *(str+i);
		*(enc+i) ^= ~(1 << 4) | 0xf0 &
			key[(j*sizeof(SECRET_CODE))%255];
		*(dec+i) = *(enc+i);
		*(dec+i) ^= ~(1 << 4) | 0xf0 &
			key[(j*sizeof(SECRET_CODE))%255];
		i++;
		if (j == strlen_p(SECRET_CODE)) j=0;
		else j++;
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
