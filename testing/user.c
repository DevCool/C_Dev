/* program to run commands as root
 * by Philip R. Simonson
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    /* declare variables */
    uid_t uid;
    FILE *file;
    char syscmd[256];
    char name[128];
    char id[32];
    unsigned char searching;

    /* if no arguments, return */
    if(argc < 2) {
        fprintf(stderr, "Usage: %s program [\"args\"]\n",argv[0]);
        return 0;
    }
    /* get user id */
    uid = getuid();
    /* open passwd file */
    if((file = fopen("/etc/passwd","rt")) == NULL) {
        fprintf(stderr, "Could not open /etc/passwd.\n");
        return 1;
    }
    /* search for uid inside of /etc/passwd */
    memset(id,0,sizeof(id));
    sprintf(id,"%d",uid);
    searching = 1;
    while(searching) {
        char *tmp;
        fgets(syscmd,256,file);
        tmp = strtok(syscmd, ":");
        if(tmp != NULL)
            strncpy(name, tmp, 128);
        while((tmp = strtok(NULL, ":")) != NULL) {
            if(strcmp(tmp, id) == 0) {
                searching = 0;
                break;
            }
        }
    }
    fclose(file);
    /* done searching, close file */

    /* set uid to super user */
    if(setuid(0)) {
        perror("setuid");
        return 1;
    }
    /* prints what user you currently are */
    printf("You are running as : %s.\n",
        (getuid() == 0) ? "SUPERUSER" : name);
    /* if/else for getting args */
    if(argc > 2) {
        /* define args and i,j */
        char *args[argc-2];
        int i, j;
        j = 0;
        for(i = 1; i <= argc; ++i) {
            args[j] = argv[i];
#if defined(DEBUG)
            printf("Arg %d: %s\n",j,args[j]);
#endif
            ++j;
        }
        /* duplicate standard input/output */
        dup(1);
        dup(2);
        /* execute program with args */
        execvp(argv[1], args);
    } else {
	char *args[] = {"",NULL};
        /* duplicate standard input/output */
        dup(1);
        dup(2);
        /* execute program with args */
        execvp(argv[1], args);
    }
    /* change back to user that executed
     * me */
    if(setuid(uid)) {
        perror("setuid");
        return 1;
    }
    /* print out what user you are */
    printf("You are running as : %s.\n",
        (getuid() == 0) ? "SUPERUSER" : name);
    return 0;
}
