/******************************************************
 * ex1.c - Example 1, how to write a CGI program in C *
 * for displaying information. On the form that re-   *
 * quests it.                                         *
 ******************************************************
 * dev: Philip "5n4k3" Simonson                       *
 ******************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* #define DEBUG */

void header(const char *mime_type);
void startup(const char *title);
void print_p(const char *paragraph);
void footer(const char *copyright);
char *getvar(char *s, const char *token, char ch);
int test(void);
void test2(void);
int countfile(const char *filename);
void writefile(const char *filename, const char *data, int iSwitch);
char *readfile(const char *filename, int i);

int main(int argc, char **argv, char **envp)
{
	header("text/html");
	startup("Simple Webpage");
	if(!test())
		test2();
	footer("Copyright (C) 2017");
	return 0;
}

int test(void)
{
	char *query, *data, *env;
	int i;

	env = getenv("QUERY_STRING");
	if(!env) {
		printf("<p>align=\"center\"><h3>Sorry but the environment variable QUERY_STRING is NULL.</h3></p>");
		return 1;
	} else {
		query = strdup(env);
		if(!query) {
			print_p("Cannot allocate memory for new QUERY_STRING...");
			return 1;
		}
		i = 0;
		do {
			
			data = getvar(query, "&", '=');
			writefile("data.dat", data, i);
			i = !i;
			while(data != NULL) {
				data = getvar(NULL, "&", '=');
				writefile("data.dat", data, i);
				i = !i;
			}
		} while(data != NULL);
		free(query);
	}

	return 0;
}

void test2(void)
{
	int i, j, lines;
	char *data;

	lines = countfile("data.dat");
	printf("<center><table><caption>User List</caption><tr><th>Usernames</th><td>Identification</td></tr>");
	i = 0;
	while(j < lines) {
		memset(data, 0, sizeof(data));
		data = readfile("data.dat", i);
		while(!i) {
			printf("<tr><th>%s</th>", data);
			memset(data, 0, sizeof(data));
			data = readfile("data.dat", i);
			printf("<td>%s</td></tr>", data);
			i = !i;
		}
	}
	printf("</table></center>");
}

int countfile(const char *filename)
{
	FILE *file;
	int lines, c;

	if((file = fopen(filename, "rt")) == NULL) {
		print_p("Cannot open file.\n");
		return -1;
	}
	lines = 0;
	while((c = fgetc(file)) != EOF)
		if(c == '\n')
			++lines;
	fclose(file);
	return lines;
}

void writefile(const char *filename, const char *data, int iSwitch)
{
	FILE *file;

	if((file = fopen(filename, "at")) == NULL) {
		print_p("Sorry cannot write the file.");
		return;
	}
	if(iSwitch == 0)
		fprintf(file, "username=%s&", data);
	else
		fprintf(file, "password=%s\n", data);
	fclose(file);
	print_p("File was appended to...");
}

char *readfile(const char *filename, int i)
{
	FILE *file;
	char data[256];

	if((file = fopen(filename, "rt")) == NULL) {
		print_p("Sorry cannot open the file.");
		return;
	}
	memset(data, 0, sizeof(data));
	if(i == 1)
		fgets(data, '\n', file);
	else
		fgets(data, '\n', file);
	fclose(file);
}

void header(const char *mime_type)
{
	printf("Content-type:%s\n\n", mime_type);
}

void startup(const char *title)
{
	printf("<!DOCTYPE HTML><html><head><title>%s</title><style>header, footer { padding: 1em; " \
	"color: #7A7A7A; background-color: #2A2A2A; font-size: 15px; clear: center; text-align: center; }</style>" \
	"</head><body bgcolor=\"#585858\">", title);
	printf("<p><header>%s</header></p>", title);
}

void print_p(const char *paragraph)
{
	printf("<P>%s</P>", paragraph);
}

void footer(const char *copyright)
{
	printf("<p><footer>%s</footer></p>", copyright);
	printf("</body></html>");
}

char *strch(char *s, char ch)
{
	while(s && *s != 0)
		if(*s++ == ch)
			return s;
	return NULL;
}

char *__getvar;

char *getvar(char *s, const char *token, char ch)
{
	char *sbeg, *send;

	sbeg = s ? s : __getvar;
	if(!sbeg)
		return NULL;
	sbeg = strtok(s, token);
	if(!sbeg) {
		__getvar = NULL;
		return NULL;
	}
	send = strch(sbeg, ch);
	if(!send)
		return NULL;
	__getvar = send;
	return send;
}

/* test() - a function for testing out my getvar function.
 */
void test3(void)
{
	char *string = "username=AUserName&password=APassWord";
	char *data, *tmp;

	tmp = strdup(string);
	data = getvar(tmp, "&", '=');
	while(data != NULL) {
		printf("data = %s\n", data);
		data = getvar(NULL, "&", '=');
	}
	free(tmp);
}
