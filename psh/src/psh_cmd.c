/*****************************************************************************
 * psh_cmd.c - this is my main command processing shell source.
 *****************************************************************************
 * by Philip R. Simonson
 *****************************************************************************
 */

/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

/* Include windows headers */
#if defined(_WIN32) || (_WIN64)
#include <windows.h>
#endif

#define CMD_BUFSIZE	2
#define CMD_TOK_COUNT	64
#define CMD_TOK_DELIMS	" \t\r\n\a"

/* ------------------------ Start of command shell ------------------------ */

char *psh_read_line(void) {
	char *buf = NULL;
	int c, i = 0;
	int size = CMD_BUFSIZE;

	if((buf = malloc(size*sizeof(char))) == NULL) {
		fprintf(stderr, "psh: Out of memory.\n");
		return NULL;
	}

	while(1) {
		c = getchar();
		if(c == EOF || c == 0x0A) {
			buf[i] = 0x00;
			return buf;
		} else {
			buf[i] = c;
		}
		++i;

		if(i >= size) {
			size += CMD_BUFSIZE;
			buf = realloc(buf, size);
			if(buf == NULL) {
				fprintf(stderr, "psh: Out of memory.\n");
				break;
			}
		}
	}
	return NULL;
}

char **psh_split_line(char *line) {
	char **tokens = NULL;
	char *token = NULL;
	int i = 0, size = CMD_TOK_COUNT;

	tokens = (char **)malloc(size * sizeof(char *));
	if(tokens == NULL) {
		fprintf(stderr, "Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, CMD_TOK_DELIMS);
	while(token != NULL) {
		tokens[i] = token;
		i++;

		if(i >= size) {
			size += CMD_TOK_COUNT;
			tokens = (char **)realloc(tokens, size * sizeof(char *));
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

int psh_launch(char **args) {
	if(execvp(args[0], args) == -1) {
		perror("psh");
	}
	return 1;
}

/* --------- Shell builtin commands ---------- */

#define CMD1_COUNT 3
#define CMD2_COUNT 4

static char *builtin_str[] = {
	"cd",
	"touch",
	"rm"
};

static char *builtin_str2[] = {
	"ls",
	"pwd",
	"help",
	"exit"
};

#define CNT_ARGS(A) (((signed int)(sizeof(A)/sizeof(char *))))

int psh_cd(char **args) {
	if(args[1] == NULL) {
		fprintf(stderr, "psh: expected argument to \"cd\" into\n");
	} else {
		if(chdir(args[1]) != 0) {
			perror("psh");
		}
	}
	return 1;
}

int psh_touch(char **args) {
	int i;

	if(args[1] == NULL) {
		fprintf(stderr, "psh: touch requires file names in order to create files.\n");
		return 1;
	}
	for(i = 1; i <= CNT_ARGS(args); i++) {
		FILE *file = NULL;
		int count = 0;
		for(i = 1; i <= CNT_ARGS(args); i++) {
			if((file = fopen(args[i], "w")) == NULL) {
				fprintf(stderr, "psh: could not create file %s.\n", args[i]);
				return 2;
			}
			fclose(file);
			++count;
		}
		printf("%d files created.\n", count);
	}
	return 1;
}

int psh_rm(char **args) {
	int i;

	if(args[1] == NULL) {
		fprintf(stderr, "psh: rm command takes arguments, to delete files.\n");
		return 1;
	}
	for(i = 1; i <= CNT_ARGS(args); i++) {
		remove(args[i]);
		printf("File [%s] removed.\n", args[i]);
	}
	return 1;
}

int psh_pwd(void) {
	char dir[BUFSIZ];
	memset(dir, 0, sizeof(dir));
	if(getcwd(dir, sizeof(dir)) == NULL) {
		fprintf(stderr, "psh: could not get current working directory.\n");
	} else {
		printf("Current working directory: %s\n", dir);
	}
	return 1;
}

int psh_ls(void) {
	DIR *d = NULL;
	struct dirent *dir = NULL;
	char dirname[BUFSIZ];
	d = opendir(".");
	if(d != NULL) {
		if(getcwd(dirname, sizeof(dirname)) != NULL) {
			printf("Directory listing of %s\n", dirname);
			while((dir = readdir(d)) != NULL)
				printf("%s\n", dir->d_name);
		} else {
			fprintf(stderr, "psh: Cannot get current working directory.\n");
		}
		free(dir);
		free(d);
	} else {
		puts("psh: Cannot list directory contents.");
	}
	return 1;
}

int psh_help(void) {
	int i;
	printf("Philip's Shell v0.02a\n************************\n");
	printf("Type program names and arguments, and press enter.\n");
	printf("Have phun with my $h3l7 :P\n");

	for(i = 0; i < CNT_ARGS(builtin_str); i++)
		printf("   %s\n", builtin_str[i]);
	for(i = 0; i < CNT_ARGS(builtin_str2); i++)
		printf("   %s\n", builtin_str2[i]);
	printf("Use documentation for other programs to see how to use them.\n");
	return 1;
}

int psh_exit(void) {
	return 0;
}

int (*builtin_func[])(char **args) = {
	&psh_cd,
	&psh_touch,
	&psh_rm
};

int (*builtin_func2[])(void) = {
	&psh_ls,
	&psh_pwd,
	&psh_help,
	&psh_exit
};

#if defined(_WIN32) || (_WIN64)
int psh_process(char **args) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char *cmdline = NULL;
	int i, max = 0;

	for(i = 0; i < CNT_ARGS(args); i++)
		max = strlen(args[i]);
	cmdline = (char *)malloc((max*sizeof(char))+1);
	if(cmdline == NULL) {
		fprintf(stderr, "psh: Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	strcpy(cmdline, args[0]);
	for(i = 1; i <= CNT_ARGS(args); i++) {
		if(args[i] == NULL)
			break;
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

int psh_execute(char **args) {
	int i;

	if(args[0] == NULL) {
		/* Empty command */
		printf("You need to enter a command.\n");
		return 1;
	}

	for(i = 0; i < CNT_ARGS(builtin_str); i++) {
		if(strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}
	for(i = 0; i < CNT_ARGS(builtin_str2); i++) {
		if(strcmp(args[0], builtin_str2[i]) == 0) {
			return (*builtin_func2[i])();
		}
	}
#if defined(_WIN32) || (_WIN64)
	return psh_process(args);
#else
	return psh_launch(args);
#endif
}

/* psh_loop() - just use this if you want my shell.
 */
void psh_loop(void) {
	char *line = NULL;
	char **args = { NULL };
	int status;

	do {
		printf("PSH >> ");
		line = psh_read_line();
		args = psh_split_line(line);
		status = psh_execute(args);
		free(line);
	} while(status);
	free(args);
}

/* ------------------------ End of my command shell ----------------------- */
