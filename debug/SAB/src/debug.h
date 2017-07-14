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

/* Message printing macros */
#define PRINT(M, ...) fprintf(stderr, M, ##__VA_ARGS__)
#define PRINT_FIXED(M) fprintf(stderr, M)

/* Check debugging macros */
#define CHECK(T, I, M, ...) if((T)) { PRINT("[" I "] : " M "\n"\
    "FILE: [%s]\nLINE: [%u]\n", ##__VA_ARGS__, __FILE__, __LINE__);\
    goto error; errno = 0; exit(errno); }
#define CHECK2(T, I, M) if((T)) { PRINT_FIXED(M); goto error;\
    errno = 0; exit(errno); }
#define CHECK_FIXED(T, I, M) CHECK2((T), I, M)
#define CHECK_MEM(P) CHECK2(!(P), "ERROR", "Out of memory.");

/* Normal debugging macros */
#define DEBUG(I, M, ...) PRINT("[" I "] : " M "\n" \
"FILE: [%s]\nLINE: [%u]\n", ##__VA_ARGS__, __FILE__, __LINE__)
#define DEBUG2(I, M) PRINT_FIXED("[" I "] : " M "\n")
#define DEBUG_FIXED(I, M) DEBUG2(I, M)
#define ERROR(T, M, ...) CHECK(T, "ERROR", M, ##__VA_ARGS__)
#define ERROR2(T, M) CHECK2(T, "ERROR", M)
#define ERROR_FIXED(T, M) ERROR2(T, M)
#define WARN(I, M, ...) DEBUG(I, M, ##__VA_ARGS__)
#define WARN_FIXED(I, M) DEBUG2("WARNING", M)

/* File handling debugging macros */
#define FOPEN_ERROR(P, name, mode) if(P == NULL) { \
P = fopen(name, mode); if(P == NULL) { \
fprintf(stderr, "[ERROR] : File was not opened.\n"); \
} else { if(NDEBUG) { DEBUG_FIXED("INFO", "File opened successfully!"); } else { \
printf("File opened successfully!\n"); } } }
#define FCLOSE_ERROR(P) ERROR_FIXED(fclose(P) == EOF, "File closing failed.")

#endif
