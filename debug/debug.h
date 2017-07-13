#ifndef __debug_h__
#define __debug_h__

/* Be sure to include these files inside your programs...
 * stdio.h, stdlib.h, string.h, errno.h
 */

#if defined(DEBUGGING)
#define NDEBUG 1
#else
#define NDEBUG 0
#endif

/* Check debugging macros */
#define CHECK(T, I, M, ...) if((T)) { fprintf(stderr, "[" I "] : " M "\n" \
    "FILE: [%s]\nLINE: [%u]\n", ##__VA_ARGS__, __FILE__, __LINE__); \
    goto error; errno = 0; exit(errno); }
#define CHECK_FIXED(T, I, M) CHECK(T, I, M, NULL)
#define CHECK_MEM(P) CHECK(!(P), "ERROR", "Out of memory.", NULL);

/* Normal debugging macros */
#define DEBUG(I, M, ...) fprintf(stderr, "[" I "] : " M "\n" \
"FILE: [%s]\nLINE: [%u]\n", ##__VA_ARGS__, __FILE__, __LINE__)
#define DEBUG_FIXED(I, M) DEBUG(I, M, NULL)
#define ERROR(T, M, ...) CHECK(T, "ERROR", M, ##__VA_ARGS__)
#define ERROR_FIXED(T, M) ERROR(T, M, NULL)
#define WARN(T, M, ...) DEBUG("WARNING", M, ##__VA_ARGS__)
#define WARN_FIXED(T, M) WARN(T, M, NULL)

/* File handling debugging macros */
#define FOPEN_ERROR(P, name, mode) if(P == NULL) { \
P = fopen(name, mode); if(P == NULL) { \
fprintf(stderr, "[ERROR] : File was not opened.\n"); \
} else { if(NDEBUG) { DEBUG_FIXED("INFO", "File opened successfully!"); } else { \
printf("File opened successfully!\n"); } } }
#define FCLOSE_ERROR(P) ERROR_FIXED(fclose(P) == EOF, "File closing failed.")

#endif
