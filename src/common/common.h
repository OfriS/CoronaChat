
#include <stdio.h>
#include <errno.h>
#include <string.h>

/*** Constants ***/

/* Return values. */
#define C_FAILURE (-1)
#define C_SUCCESS (0)

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

