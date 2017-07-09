/*****************************************************************************
 * psh_cmd.c - this is my main command processing shell source.
 *****************************************************************************
 * by Philip R. Simonson
 *****************************************************************************
 */

#if defined(_WIN32)
/* Include Windows headers */
#include <windows.h>
#elif defined(linux)
/* Include Linux headers */
#include <sys/wait.h>
#include <sys/types.h>
#include <termcap.h>
#endif

/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define CMD_BUFSIZE	2
#define CMD_TOK_COUNT	64
#define CMD_TOK_DELIMS	" \t\r\n\a"
#define NDEBUG

#if defined(linux)
extern int mkdir(const char *path);
extern int fileno(FILE *fp);
#endif

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

char **psh_split_line(char *line, int *argcnt) {
  char **tokens = NULL;
  char *token = NULL;
  int i = 0, size = CMD_TOK_COUNT;

  tokens = (char **)malloc(size * sizeof(char *));
  if(tokens == NULL) {
    fprintf(stderr, "Cannot allocate memory.\n");
    exit(EXIT_FAILURE);
  }

  *argcnt = 0;
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
  *argcnt = i;
  return tokens;
}

#if defined(linux)
int psh_launch(char **args) {
  int pid = -1, status;

  pid = fork();
  if(pid < 0) {
    /* error forking to background */
    perror("psh");
  } else if(pid == 0) {
    /* wait for child process to launch */
    if(execvp(args[0], args) == -1) {
      perror("psh");
    }
    exit(EXIT_FAILURE);
  } else {
    /* parent process waits for child */
    do {
      waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}
#endif

/* ---------- Shell builtin commands ----------- */

static char *builtin_str[] = {
  "psh_echo",
  "cd",
  "touch",
  "rm",
  "rmdir",
  "mkdir",
  "deltree"
};

static char *builtin_str2[] = {
  "clear",
  "ls",
  "pwd",
#if defined(_WIN32)
  "abort",
#endif
  "reboot",
  "shutdown",
  "help",
  "exit"
};

static char *builtin_help[] = {
  "echos text to the screen.",
  "change directory to a different one.",
  "create new files without data inside.",
  "delete a file from the system.",
  "remove an empty directory.",
  "make a new directory.",
  "delete an entire directory tree.",
  "clears the terminal screen",
  "list directory structure.",
  "print working directory.",
#if defined(__WIN32) || (_WIN64)
  "aborts shutdown of the computer",
#endif
  "restarts the computer",
  "turns off the computer",
  "this command help message.",
  "quits this command shell."
};

/* --------- Below are the functions ----------- */

#define CNT_ARGS(A) (((signed int)(sizeof(A)/sizeof(char *))))

int psh_print(char **args, int argcnt) {
#if !defined(NDEBUG)
  printf("Argument Count: %d\n", argcnt);
#endif
  if(argcnt < 2) {
   fprintf(stderr, "psh: expected argument(s) to echo to screen.\n");
  } else {
    int i = 1;
    while(i < argcnt) {
      if(args[i] != NULL)
        printf("%s ", args[i]);
      ++i;
    }
    putchar('\n');
  }
  return 1;
}

int psh_cd(char **args, int argcnt) {
#if !defined(NDEBUG)
  printf("Argument Count: %d\n", argcnt);
#endif
  if(argcnt < 2) {
    fprintf(stderr, "psh: expected one argument to \"cd\" into\n");
  } else {
    if(chdir(args[1]) != 0) {
      perror("psh");
    }
  }
  return 1;
}

int psh_touch(char **args, int argcnt) {
  int i, count = 0;

#if !defined(NDEBUG)
  printf("Argument Count: %d\n", argcnt);
#endif
  if(argcnt < 2) {
    fprintf(stderr, "psh: touch requires file names in order to create files.\n");
    return 1;
  }
  for(i = 1; i < argcnt; i++) {
    FILE *file = NULL;
    if((file = fopen(args[i], "w")) == NULL) {
      fprintf(stderr, "psh: could not create file %s.\n", args[i]);
      return 2;
    }
    fclose(file);
    ++count;
  }
  printf("%d files created.\n", count);
  return 1;
}

int psh_rm(char **args, int argcnt) {
  int i;

#if !defined(NDEBUG)
  printf("Argument Count: %d\n", argcnt);
#endif
  if(argcnt < 2) {
    fprintf(stderr, "psh: rm command takes arguments, to delete files.\n");
    return 1;
  }
  for(i = 1; i < argcnt; i++) {
    remove(args[i]);
    printf("File [%s] removed.\n", args[i]);
  }
  return 1;
}

int psh_rmdir(char **args, int argcnt) {
  int i;

#if !defined(NDEBUG)
  printf("Argument Count: %d\n", argcnt);
#endif
  if(argcnt < 2) {
    fprintf(stderr, "psh: rmdir command takes arguments, to\ndelete directories.\n");
    return 1;
  }
  for(i = 1; i < argcnt; i++)
    if(rmdir(args[i]) != 0) {
      fprintf(stderr, "psh: failed to delete %s directory.\n", args[i]);
    } else {
      printf("Directory [%s] deleted.\n", args[i]);
    }
  return 1;
}

int psh_mkdir(char **args, int argcnt) {
  int i;

#if !defined(NDEBUG)
  printf("Argument Count: %d\n", argcnt);
#endif
  if(argcnt < 2) {
    fprintf(stderr, "psh: mkdir command takes arguments, to\ncreate directories.\n");
    return 1;
  }
  for(i = 1; i < argcnt; i++)
    if(mkdir(args[i]) != 0) {
      fprintf(stderr, "psh: failed to make %s directory.\n", args[i]);
    } else {
      printf("Directory [%s] created.\n", args[i]);
    }
  return 1;
}

int psh_deltree(char **args, int argcnt) {
  DIR *d = NULL;
  struct dirent *dir;
  int i;

#if !defined(NDEBUG)
  printf("Argument Count: %d\n", argcnt);
#endif
  if(argcnt < 2) {
    fprintf(stderr, "psh: deltree command takes arguments, to\nwipe directories.\n");
    return 1;
  }
  for(i = 1; i < argcnt; i++) {
    d = opendir(args[i]);
    if(d != NULL) {
      while((dir = readdir(d)) != NULL) {
        char *file = NULL;
	if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
          continue;
        file = calloc(1, strlen(args[i])+strlen(dir->d_name)+1);
        if(file == NULL) {
          fprintf(stderr, "psh: cannot alloc memory for filename.\n");
          return 1;
        }
        sprintf(file, "%s/%s", args[i], dir->d_name);
        if(unlink(file) != 0) {
          fprintf(stderr, "psh: Cannot unlink file %s\n", file);
        }
        free(file);
      }
      if(rmdir(args[i]) != 0) {
        fprintf(stderr, "Cannot delete directory %s\n", args[i]);
      } else {
        printf("Directory %s deleted.\n", args[i]);
      }
      closedir(d);
    } else {
      printf("Directory %s does not exist.\n", args[i]);
    }
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

void cls(HANDLE hConsole) {
  COORD coordScreen = { 0, 0 };
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwConSize;

  /* Get the number of char cells */
  if(!GetConsoleScreenBufferInfo(hConsole, &csbi))
    return;

  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

  /* Fill the entire screen */
  if(!FillConsoleOutputCharacter(hConsole,
                                 ' ',
                                 dwConSize,
                                 coordScreen,
                                 &cCharsWritten))
    return;

  /* put the cursor at its home coord */
  SetConsoleCursorPosition(hConsole, coordScreen);
}

int psh_clear(void) {
#if defined(_WIN32)
  cls(GetStdHandle(STD_OUTPUT_HANDLE));
#else
  char buf[1024];
  char *str = NULL;

  tgetent(buf, getenv("TERM"));
  str = tgetstr("cl", NULL);
  fputs(str, stdout);
#endif
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

#if defined(__WIN32) || (_WIN64)
int psh_abort(void) {
  if(!AbortSystemShutdown(NULL)) {
    printf("psh: Cannot abort system shutdown error code [%lu]\n",
        GetLastError());
    return 1;
  }
  printf("Shutdown successfully aborted.\n");
  return 1;
}
#endif

int psh_reboot(void) {
#if defined(_WIN32)
  if(InitiateSystemShutdown(NULL, "Standard PC shutdown sequence.",
      10, FALSE, TRUE) == 0) {
    printf("psh: Cannot reboot the system error code [%lu]\n",
      GetLastError());
    return 1;
  }
  printf("Rebooting system...\n");
#else
  uid_t uid;
  uid = getuid();
  if(uid != 0)
    execl("/usr/bin/sudo", "/sbin/shutdown", "-r", "now", NULL);
  else
    execl("/usr/bin/shutdown", "/sbin/shutdown", "-r", "now", NULL);
#endif
  return 1;
}

int psh_shutdown(void) {
#if defined(_WIN32)
  if(InitiateSystemShutdown(NULL, "Standard PC shutdown sequence.",
      10, FALSE, FALSE) == 0) {
    printf("psh: Cannot shutdown the system down error code [%lu]\n",
      GetLastError());
    return 1;
  }
  printf("Turning off system...\n");
#else
  uid_t uid;
  uid = getuid();
  if(uid != 0)
    execl("/usr/bin/sudo", "/sbin/shutdown", "-h", "-P", "now", NULL);
  else
    execl("/usr/bin/shutdown", "/sbin/shutdown", "-h", "-P", "now", NULL);
#endif
  return 1;
}

int psh_help(void) {
  int i, j;
  printf("Philip's Shell v0.02a by Philip Ruben Simonson\n"\
    "************************\n");
  printf("Type program names and arguments, and press enter.\n");
  printf("Have phun with my $h3l7 :P\n");

  j = 0;
  for(i = 0; i < CNT_ARGS(builtin_str); i++, j++)
    if(strlen(builtin_str[i]) == 2)
      printf("   %s        - %s\n", builtin_str[i], builtin_help[j]);
    else if(strlen(builtin_str[i]) == 5)
      printf("   %s     - %s\n", builtin_str[i], builtin_help[j]);
    else if(strlen(builtin_str[i]) == 7)
      printf("   %s   - %s\n", builtin_str[i], builtin_help[j]);
    else if(strlen(builtin_str[i]) == 8)
      printf("   %s  - %s\n", builtin_str[i], builtin_help[j]);
    else if(strlen(builtin_str[i]) == 3)
      printf("   %s       - %s\n", builtin_str[i], builtin_help[j]);
    else
      printf("   %s      - %s\n", builtin_str[i], builtin_help[j]);

  j = CNT_ARGS(builtin_str);
  for(i = 0; i < CNT_ARGS(builtin_str2); i++, j++)
    if(strlen(builtin_str2[i]) == 2)
      printf("   %s        - %s\n", builtin_str2[i], builtin_help[j]);
    else if(strlen(builtin_str2[i]) == 5)
      printf("   %s     - %s\n", builtin_str2[i], builtin_help[j]);
    else if(strlen(builtin_str2[i]) == 7)
      printf("   %s   - %s\n", builtin_str2[i], builtin_help[j]);
    else if(strlen(builtin_str2[i]) == 6)
      printf("   %s    - %s\n", builtin_str2[i], builtin_help[j]);
    else if(strlen(builtin_str2[i]) == 8)
      printf("   %s  - %s\n", builtin_str2[i], builtin_help[j]);
    else if(strlen(builtin_str2[i]) == 3)
      printf("   %s       - %s\n", builtin_str2[i], builtin_help[j]);
    else
      printf("   %s      - %s\n", builtin_str2[i], builtin_help[j]);

  printf("Use documentation for other programs to see how to use them.\n");
  return 1;
}

int psh_exit(void) {
  return 0;
}

int (*builtin_func[])(char **args, int argcnt) = {
  &psh_print,
  &psh_cd,
  &psh_touch,
  &psh_rm,
  &psh_rmdir,
  &psh_mkdir,
  &psh_deltree
};

int (*builtin_func2[])(void) = {
  &psh_clear,
  &psh_ls,
  &psh_pwd,
#if defined(__WIN32) || (_WIN64)
  &psh_abort,
#endif
  &psh_reboot,
  &psh_shutdown,
  &psh_help,
  &psh_exit
};

#if defined(_WIN32)
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

int psh_execute(char **args, int argcnt) {
  int i;

  if(args[0] == NULL) {
    /* Empty command */
    printf("You need to enter a command.\n");
    return 1;
  }

  for(i = 0; i < CNT_ARGS(builtin_str); i++) {
    if(strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args, argcnt);
    }
  }
  for(i = 0; i < CNT_ARGS(builtin_str2); i++) {
    if(strcmp(args[0], builtin_str2[i]) == 0) {
      return (*builtin_func2[i])();
    }
  }
#if defined(_WIN32)
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
  int status, count = 0;

  do {
    printf("PSH >> ");
    line = psh_read_line();
    args = psh_split_line(line, &count);
    status = psh_execute(args, count);
    free(line);
  } while(status);
  free(args);
}

/* ------------------------ End of my command shell ----------------------- */
