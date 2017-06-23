#include <stdio.h>
#include <string.h>

int get_filename(char *path, char *fname, char *ext);

int main(int argc, char **argv) {
    char filename[256];
    char ext[5];
    int res;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 0;
    }

    memset(filename, 0, sizeof(filename));
    memset(ext, 0, sizeof(ext));
    res = get_filename(argv[1], filename, ext);
    if(res < 0)
        fprintf(stderr, "Error: Couldn't get filename...\nFile data: %s\n",
                argv[1]);

    printf("Full path: %s\nFilename : %s.%s\n", argv[1],
            filename, ext);

    return res;
}

int get_filename(char *path, char *fname, char *ext) {
    char found = 0;
    char *tmp, *dot;
    char *end;

    end = path;
    while(*end++ != 0);
    if((tmp = strrchr(path, '/')) != NULL)
        found = 1;
    if((dot = strrchr(path, '.')) != NULL) {
        if(found)
            strncpy(fname, &tmp[1], dot-tmp-1);
        else
            strncpy(fname, path, dot-path);
        strncpy(ext, &(*(dot+1)), end-dot);
    } else return 1;

    return 0;
}
