
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/*** Constants ***/
/* Value of an invalid file descriptor. */
#define INVALID_FILE_DESCRIPTOR (-1)

/* Return values. */
#define C_FAILURE (-1)
#define C_SUCCESS (0)

#define CLOSE(__fd) \
do { \
    if (INVALID_FILE_DESCRIPTOR != (__fd)) { \
        (void)close(__fd); \
    } \
} while (0)

#define HEAPFREE(__buffer) \
do { \
    if (NULL != (__buffer)) { \
        (void)free(__buffer); \
    } \
} while (0)

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt "\n", \
        __FILE__, __LINE__, __func__, ##args)
#else
    #define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

#ifdef DEBUG
#define DEBUG_PRINT_WITH_ERRNO(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt ":%s\n", \
        __FILE__, __LINE__, __func__, ##args, strerror(errno))
#else
#define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

