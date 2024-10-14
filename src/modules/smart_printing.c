#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>



#define DEBUG(var, fmt) fprintf(stderr, "%s=" fmt "\n", (#var), (var))

#define PRINT(var) _Generic((var),        \
             int     : DEBUG(var, "%d" ), \
             size_t  : DEBUG(var, "%lu"), \
             bool    : DEBUG(var, "%b" ), \
             char    : DEBUG(var, "%c" ), \
             char*   : DEBUG(var, "%s" ), \
             void*   : DEBUG(var, "%p" ), \
             default : assert(!"unknown type"));
