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

int main(int argc, char **argv, char **envp)
{
	char *query, *data, *env;

	header("text/html");
	startup("Simple Webpage");
	env = getenv("QUERY_STRING");
	if(!env) {
		printf("<p>align=\"center\"><h3>Sorry but the environment variable QUERY_STRING is NULL.</h3></p>");
		goto end_code;
	} else {
		query = malloc(strlen(env)+1);
		if(!query) {
			print_p("Cannot allocate memory for new QUERY_STRING...");
			goto end_code;
		}
		strncpy(query, env, strlen(env)+1);
		printf("<p align=\"center\"><table><caption>User List</caption><tr><th>Usernames</th><td>Identification</td></tr>");
		if((data = strtok(query, "&"))) {
			printf("<tr><th>%s</th>", data);
		}
		if((data = strtok(NULL, "&"))) {
			printf("<td>%s</td></tr>", data[1]);
		}
		printf("</table></p>");
		free(query);
	}

end_code:
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

void print_p(const char *paragraph)
{
	printf("<P>%s</P>", paragraph);
}

void footer(const char *copyright)
{
	printf("<p><footer>%s</footer></p>", copyright);
	printf("</body></html>");
}
