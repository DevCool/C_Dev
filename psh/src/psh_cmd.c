/*****************************************************************************
 * psh_cmd.c - this is my main command processing shell source.
 *****************************************************************************
 * by Philip R. Simonson
 *****************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#define CMD_BUFSIZE	1024
#define CMD_TOK_COUNT	64
#define CMD_TOK_DELIMS	" \t\r\n\a"

/* ------------------------ Start of command shell ------------------------ */

int psh_read_line(char *s, int size) {
	int c, i;

	for(i = 0; (c = getchar()) != EOF && c != 0x0A; )
		if(i < size-1) {
			if(s[i] == 0x08) {
				s[i] = 0x00;
				--i;
				continue;
			}
			s[i] = c;
			++i;
		}
	if(c == 0x0A) {
		s[i] = c;
		++i;
	}
	s[i] = 0x00;
	return i;
}

char **psh_split_line(char *line, int *argcnt) {
	char **tokens = NULL;
	char *token = NULL;
	int i, size = CMD_TOK_COUNT;

	tokens = (char **)malloc(size * sizeof(char *));
	if(tokens == NULL) {
		fprintf(stderr, "Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, CMD_TOK_DELIMS);
	i = 0;
	*argcnt = 0;
	while(token != NULL) {
		tokens[i] = token;
		++i;
		*argcnt = i;

		if(i >= size) {
			size += CMD_TOK_COUNT;
			tokens = realloc(tokens, size * sizeof(char *));
			if(tokens == NULL) {
				fprintf(stderr, "Out of memory.\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, CMD_TOK_DELIMS);
	}
	tokens[i] = NULL;
	return tokens;
}

int psh_launch(char **args, int argcnt) {
	if(execvp(args[0], args) == -1) {
		perror("psh");
	}
	return 1;
}

/* --------- Shell builtin commands ---------- */

static char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int psh_num_builtins(void) {
	return sizeof(builtin_str) / sizeof(char *);
}

int psh_cd(char **args) {
	if(strcmp(args[1], "") == 0) {
		fprintf(stderr, "psh: expected argument to \"cd\" into\n");
	} else {
		if(chdir(args[1]) != 0) {
			perror("psh");
		}
	}
	return 1;
}

int psh_help(char **args) {
	int i;
	printf("Philip's Shell\n");
	printf("Type program names and arguments, and press enter.\n");
	printf("Have phun with my $h3l7 :P\n");

	for(i = 0; i < psh_num_builtins(); i++)
		printf("   %s\n", builtin_str[i]);
	printf("Use documentation for other programs to see how to use them.\n");
	return 1;
}

int psh_exit(char **args) {
	return 0;
}

int (*builtin_func[])(char **args) = {
	&psh_cd,
	&psh_help,
	&psh_exit
};

#if defined(_WIN32) || (_WIN64)
int psh_process(char **args, int argcnt) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char *cmdline = NULL;
	int i, max = 0;

	for(i = 0; i < argcnt; i++)
		max = strlen(args[i]);
	cmdline = (char *)malloc((argcnt*(max*sizeof(char)))+1);
	if(cmdline == NULL) {
		fprintf(stderr, "psh: Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	strcpy(cmdline, args[0]);
	for(i = 1; i < argcnt; i++) {
		strcat(cmdline, " ");
		strcat(cmdline, args[i]);
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_SHOW;

	if(!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		fprintf(stderr, "psh: Could not create process.\n");
		free(cmdline);
		return -1;
	}
	free(cmdline);

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 1;
}
#endif

int psh_execute(char **args, int argcnt) {
	int i;

	if(strcmp(args[0], "") == 0) {
		/* Empty command */
		printf("You did not enter any data.\n");
		return 1;
	}

	for(i = 0; i < psh_num_builtins(); i++)
		if(strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
#if defined(_WIN32) || (_WIN64)
	return psh_process(args, argcnt);
#else
	return psh_launch(args, argcnt);
#endif
}

/* psh_loop() - just use this if you want my shell.
 */
void psh_loop(void) {
	char line[CMD_BUFSIZE];
	char **args = { NULL };
	int status, argcnt;

	do {
		printf("PSH >> ");
		psh_read_line(line, CMD_BUFSIZE);
		args = psh_split_line(line, &argcnt);
		status = psh_execute(args, argcnt);
	} while(status);
	free(args);
}

/* ------------------------ End of my command shell ----------------------- */
