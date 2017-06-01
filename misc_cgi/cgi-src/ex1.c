#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void header(const char *mime_type);
void startup(const char *title);
void title(const char *title);
void footer(const char *copyright);
char *getvartext(char *query, char ch, const char *token);

int main(int argc, char **argv, char **envp)
{
	char buffer[BUFSIZ];
	char *query;
	char *data;
	int i;

	header("text/html");
	startup("Simple Webpage");
	printf("<p align=\"center\"><table><caption>User List</caption><tr><th>Usernames</th><td>Identification</td></tr>");
	if(query = getenv("QUERY_STRING")) {
		strncpy(buffer, query, sizeof(buffer));
		if((data = getvartext(buffer, '=', "&")) != NULL) {
			printf("<tr><th>%s</th>", data);
			free(data);
		}
		if((data = getvartext(NULL, '=', "&")) != NULL) {
			printf("<td>%s</td></tr>", data[1]);
			free(data);
		}
	}
	printf("</table></p>");
	footer("Copyright (C) 2017");
	return 0;
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

void footer(const char *copyright)
{
	printf("<p><footer>%s</footer></p>", copyright);
	printf("</body></html>");
}

char *strch(const char *buf, char ch)
{
	char *tmp, *a;
	int i;

	tmp = realloc(NULL, strlen(buf)+1);
	if(!tmp) return NULL;
	a = tmp;
	while(*(buf+i) != 0) {
		++i;
		if(*(buf+i) == ch)
			break;
	}
	while(*(buf+i) != 0) {
		*a++ = *(buf+i);
		++i;
	}
	return tmp;
}

char *__qstr;

char *getvartext(char *query, char ch, const char *token)
{
	char *tmp, *buf, *s;

	s = query ? query : __qstr;
	if(!s)
		return NULL;
	tmp = strtok(query, token);
	if(!tmp) {
		__qstr = NULL;
		return NULL;
	}
	buf = strch(tmp, ch);
	if(!buf)
		return NULL;
	__qstr = s;
	return s;
}
