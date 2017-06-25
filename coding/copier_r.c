/**************************************************
 * copier_r.c - Simple recursive copy functions
 * by Philip "5n4k3" Simonson
 **************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK_SIZE 1024

size_t copy_data(char *src, char *dest, size_t index);
size_t copy_file(FILE *fin, FILE *fout);

int main(int argc, char *argv[]) {
    FILE *fin, *fout;
    size_t total_bytes = 0;
    char *src = "Hello world!\n";
    char dest[14];

    memset(dest, 0, sizeof(dest));
    printf("Size of data copied: %u\n",
            copy_data(src, dest, 0));
    printf("Data Original: %s\nData Copied: %s\n",
            src, dest);

    if(argc == 3) {
        if((fin = fopen(argv[1], "rb")) == NULL) {
            perror("fopen(): ");
            return 1;
        }
        if((fout = fopen(argv[2], "wb")) == NULL) {
            perror("fopen(): ");
            return 1;
        }
        total_bytes = copy_file(fin, fout);
        if(total_bytes == -1) {
            fprintf(stderr, "Error: File copy failed.\n");
            fclose(fin);
            fclose(fout);
            return 1;
        }
        fclose(fin);
        fclose(fout);
        printf("Total bytes: %lu\nFile copied successfully!\n",
                total_bytes);
    } else {
        puts("You didn't put any arguments, so not copying file.");
    }
    return 0;
}

size_t copy_data(char *src, char *dest, size_t index) {
    dest[index] = src[index];
    if(src[index] == '\0')
        return index;
    copy_data(src, dest, index + 1);
}

size_t copy_file(FILE *fin, FILE *fout) {
    char data[CHUNK_SIZE];
    static size_t total_bytes = 0;
    int bytesRead;
    int bytesWritten;

    bytesRead = read(fileno(fin), data, CHUNK_SIZE);
    if(bytesRead > 0)
        bytesWritten = fwrite(data, bytesRead, 1, fout);
    if(bytesWritten == bytesRead)
        return total_bytes;
    else
        total_bytes += bytesWritten;
    copy_file(fin, fout);
}
