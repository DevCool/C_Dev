#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    uid_t uid;
    FILE *file;
    char syscmd[256];
    char name[128];
    char id[32];
    unsigned char searching;

    if(argc < 2) {
        fprintf(stderr, "Usage: %s program [\"args\"]\n",argv[0]);
        return 0;
    }
    uid = getuid();
    if((file = fopen("/etc/passwd","rt")) == NULL) {
        fprintf(stderr, "Could not open /etc/passwd.\n");
        return 1;
    }
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

    if(setuid(0)) {
        perror("setuid");
        return 1;
    }
    printf("You are running as : %s.\n",
        (getuid() == 0) ? "SUPERUSER" : name);
    if(argc > 2) {
        char *args[argc-2];
        int i, j;
        j = 0;
        for(i = 2; i <= argc; ++i) {
            args[j] = argv[i];
            printf("Arg %d: %s\n",j,args[j]);
            ++j;
        }
        args[j] = NULL;
        execvp(argv[1], args);
    } else {
	char *args[] = {"",NULL};
        execvp(argv[1], args);
    }
    if(setuid(uid)) {
        perror("setuid");
        return 1;
    }
    printf("You are running as : %s.\n",
        (getuid() == 0) ? "SUPERUSER" : name);
    return 0;
}
