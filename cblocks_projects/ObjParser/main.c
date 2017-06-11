/*
 **************************************
 * A program to login to itself and
 * to type some stuff.
 **************************************
 * by Philip R. Simonson
 **************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#include <windows.h>

#define ASSERT(R, T, M, ...) ({\
                        if((T)) { fprintf(stderr, "Line [%d] File [%s]\nProgrammer Defined : %s\n", __LINE__, __FILE__, (!(M)) ? \
                                    strerror(errno) : M);\
                        if(!(__VA_ARGS__)) { free(__VA_ARGS__); }\
                        errno = 0;\
                        exit(R); } })
#define assert(R, T, M) ASSERT(R, T, M, NULL)
#define check_mem(P) ASSERT(-2, P == NULL, "Error: Pointer is NULL (void*)0.", P)

void
wait(sec)
int sec;
{
    clock_t start = clock();
    while((clock()-start)/CLOCKS_PER_SEC <= sec);
}

#define BUFSIZE 2

char*
getln(a)
char a;
{
    int size = BUFSIZE;
    int pos = 0;
    char *buf = NULL;
    char c;

    buf = realloc(buf, size); /* realloc frees memory before it makes a new block */
    check_mem(buf);

    while(1) {
        c = getchar();
        if(c == -1 || c == 10) {
            if(pos==size||a) {
                size += BUFSIZE+1;
                buf = realloc(buf, size);
                check_mem(buf);
                *(buf+pos) = 13;
                *(buf+pos+1) = 0;
            } else {
                *(buf+pos) = 0;
            }
            return buf;
        } else {
            *(buf+pos) = c;
        }
        ++pos;

        if(pos >= size) {
            size += BUFSIZE;
            buf = realloc(buf, size);
            check_mem(buf);
        }
    }
    return NULL;
}

#define LOGIN_CODE "\x50\x52\x53\x00" /* PRS */
#define LOGIN_TIME (60*1)

struct _tagTHREADS {
    HANDLE hThread;
    HANDLE hThread2;
    char t1_terminated;
    char *buf;
};
typedef struct _tagTHREADS LoginThreads;

DWORD
login_thread(lpVoid)
LPVOID lpVoid;
{
    LoginThreads* ltMain = lpVoid;
    char *buf = ltMain->buf;
    while(1) {
        printf("Enter your passcode: ");
        buf = getln(0);

        if(strcmp(buf, LOGIN_CODE)==0) {
            if(TerminateThread(ltMain->hThread2, 0)) {
                break;
            }
        } else {
            printf("You're wrong!\n");
        }
    }

    if(buf != NULL) free(buf);
    return S_OK;
}

DWORD
login_timer(lpVoid)
LPVOID lpVoid;
{
    LoginThreads *ltMain = lpVoid;
    HANDLE thread = ltMain->hThread;
    wait(LOGIN_TIME);
    if(!TerminateThread(thread, 0))
        return E_FAIL;
    ltMain->t1_terminated = 1;
    printf("\nWarning: Time expired... program has ended.\n");
    return S_OK;
}

int do_something();

int
main(argc, argv)
int argc;
char **argv;
{
    LoginThreads ltMain;
    HANDLE hThread, hThread2;
    char *buf = NULL;

    printf("You have %d minutes to enter the passcode correctly.\n", LOGIN_TIME/60);
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)login_thread, &ltMain, 0, 0);
    hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)login_timer, &ltMain, 0, 0);

    memset(&ltMain, 0, sizeof(LoginThreads));
    ltMain.buf = buf;
    ltMain.hThread = hThread;
    ltMain.hThread2 = hThread2;

    while(WaitForSingleObject(hThread2, 0) != 0 ||
          WaitForSingleObject(hThread, 0) != 0);

    if(WaitForSingleObject(hThread, 0) == 0 &&
       ltMain.t1_terminated == 0) printf("Login successful!\n");
    if(ltMain.t1_terminated == 1) goto time_expired;
    if(buf != NULL) free(buf);
    return do_something();

time_expired:
    if(buf != NULL) free(buf);
    return 0;
}

BOOL
chcol(wAttr)
WORD wAttr;
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    return SetConsoleTextAttribute(hCon, wAttr);
}

void
wait2(msec)
int msec;
{
    clock_t start = clock();
    while((clock()-start)/CLOCKS_PER_SEC*1000 < msec);
}

int
put(c, fp)
int c;
FILE *fp;
{
    char ch;

    ch = c;
    write(fp->_file, &ch, 1);
    return(c);
}

void
typer(s)
const char *s;
{
    int len = strlen(s);
    int i = 0;
    while(i < len) {
        put(*s, stdout);
        /* putchar(0x07);  / (nevermind sounds bad on windows 10.) bell character */
        wait2(250);
        ++i;
        ++s;
    }
}

int
do_something()
{
    /* put your code here, modify LOGIN_TIME and LOGIN_CODE defines. */
    chcol(0x000A);
    typer("This is something very nice to play\nwith. It's pretty cool, or atleast I think so.\n");

    chcol(0x0007);
    printf("Goodbye.\n");
    return 0;
}
