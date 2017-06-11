#include <stdio.h>
#include <unistd.h>

#define DATAFILE "data.dat"

void header(const char *mime_type);
void startup(const char *title);
void footer(const char *footer);

int main(int argc, char *argv[], char **envp)
{
	FILE *file;
	char cwd[256];

	header("text/html");
	startup("Simple Webpage #3");
	if(getcwd(cwd, sizeof(cwd)) == NULL) {
		printf("<p align=\"center\">Couldn't get current working directory.</p>");
	} else {
		strncat(cwd, "\\", sizeof(cwd));
		strncat(cwd, DATAFILE, sizeof(cwd));
		if((file = fopen(cwd, "wt")) == NULL) {
			printf("Couldn't create %s file for some reason.", DATAFILE);
		} else {
			fclose(file);
		}
		printf("<p align=\"center\">Data records file cleared!</p>");
	}
	footer("Copyright (C)2017, 5n4k3");
	return 0;
}

void header(const char *mime_type)
{
	printf("Content-type:%s\n\n", mime_type);
}

void startup(const char *title)
{
	printf("<!DOCTYPE HTML><html><head><title>%s</title><style>header, footer { padding: 1em; color: #8A8A8A; background-color: #2A2A2A; "\
		"text-align: center; clear: center; }</style></head><body bgcolor=\"#5B5B5B\"><p><header>%s</header></p>", title, title);
}

void footer(const char *footer)
{
	printf("<p><footer>%s</footer></p>", footer);
	printf("</body></html>");
}
