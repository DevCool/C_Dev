#define SIMPLE_HASH "ZEROCOOL_HSH000075937"
#define ENDING_FILE "zC_FILEV_01"
#define MAX_FILENAME 512

#include <stdio.h>

int strlen (const char *str)
{
	int cnt = 0;
	while (*str++ != 0) {
		++cnt;
	}
	return cnt;
}

int encrypt (const char *filename)
{
	int i = 0;
	char c;
	FILE *fp = 0;
	FILE *newFp = 0;
	char fname[MAX_FILENAME];

	fp = fopen (filename, "rb");
	if (fp == 0) {
		printf ("Could open file: %s.\n", filename);
		return 1;
	}
	sprintf (fname, "%s.enc", filename);
	newFp = fopen (fname, "wb");
	if (newFp == 0) {
		printf ("Couldn't open file: %s.\n", fname);
		return 1;
	}

	fseek (fp, 0, SEEK_SET);
	fseek (newFp, 0, SEEK_SET);

	while ((c = fgetc (fp)) != EOF) {
		c = c^SIMPLE_HASH[i % (sizeof (SIMPLE_HASH) / sizeof (char))];
		fputc (c, newFp);
		i++;
		if (i == strlen (SIMPLE_HASH))
			i = 0;
	}

	fclose (fp);
	fclose (newFp);

	printf ("Successfully wrote file:         [ %s ]\n", fname);
	fflush (0);

	return 0;
}

int decrypt (const char *filename)
{
	int i = 0;
	char c;
	FILE *fp = 0;
	FILE *newFp = 0;
	char fname[MAX_FILENAME];

	fp = fopen (filename, "rb");
	if (fp == 0) {
		printf ("Couldn't open file: %s.\n", filename);
		return 1;
	}
	sprintf (fname, "%s.dec", filename);
	newFp = fopen (fname, "wb");
	if (newFp == 0) {
		printf ("Couldn't open file: %s.\nFor writing operation.\n",
			fname);
		return 1;
	}

	fseek (fp, 0, SEEK_SET);
	fseek (newFp, 0, SEEK_SET);

	while ((c = fgetc (fp)) != EOF) {
		c = c^SIMPLE_HASH[i % (sizeof (SIMPLE_HASH) / sizeof (char))];
		fputc (c, newFp);
		i++;
		if (i == strlen (SIMPLE_HASH))
			i = 0;
	}

	fclose (fp);
	fclose (newFp);

	printf ("Successfully wrote file:           [ %s ]\n", fname);
	fflush (0);

	return 0;
}

int prglogo (void)
{
	puts (
		"    //\\\\         ||\n"
		"   //  \\\\        ||             /||||\\        /|||||\\\n"
		"  ||    ||      /||\\           ||    ||      ||    |/\n"
		"  ||====||    ||||||||         ||||||/       ||\n"
		"  ||    ||      \\||/           ||            ||\n"
		"  ||    ||       ||   |\\       ||     |.     ||    |\\\n"
		"  ||    ||        \\\\==//        \\||||||/      \\||||/\n"
	);
	return 0;
}

