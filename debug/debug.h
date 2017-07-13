#ifndef __debug_h__
#define __debug_h__

/* Be sure to include these files inside your programs...
 * stdio.h, stdlib.h, string.h, errno.h
 */

/* Check debugging macros */
#define CHECK(T, I, M, ...) if((T)) { fprintf(stderr, "[" I "] : " M "\n"\
	"FILE: [%s]\nLINE: [%u]\n", ##__VA_ARGS__, __FILE__, __LINE__);\
	goto error; errno = 0; exit(errno); }
#define CHECK_MEM(P) CHECK(!(P), "ERROR", "Out of memory.");

/* Normal debugging macros */
#define DEBUG(I, M, ...) fprintf(stderr, "[" I "] : " M "\n"\
	"FILE: [%s]\nLINE: [%u]\n", ##__VA_ARGS__, __FILE__, __LINE__);
#define ERROR(T, M, ...) CHECK((T), "ERROR", M, ##__VA_ARGS__);
#define WARN(T, M, ...) DEBUG("WARNING", M, ##__VA_ARGS__);

/* File handling debugging macros */
#define FOPEN_ERROR(P, name, mode) if(!(P)) { P = fopen(name, mode); \
if(P == NULL) { fprintf(stderr, "[ERROR] : %s.\n" \
"FILE: [%s]\nLINE: [%u]\n", strerror(errno), __FILE__, __LINE__); \
exit(errno); } else { ERROR(fclose(P) == EOF, "Closing file failed."); \
errno = 0; exit(errno); } } else { fprintf(stderr, "File must already " \
"be open."); }

#define FCLOSE_ERROR(P) if(!(P)) { fprintf(stderr, "[ERROR] : %s.\n" \
"FILE: [%s]\nLINE: [%u]\n", strerror(errno), __FILE__, __LINE__); \
exit(errno); } else { ERROR(fclose(P) == EOF, "Closing file failed."); \
errno = 0; exit(errno); }

#endif