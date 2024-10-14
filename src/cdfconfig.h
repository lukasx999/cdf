#define CDF_HEADER
#include "cdf.c"
SETUP()

// #define EXPORT(...) assert(true)

static const char
*cc                  = "gcc",
*translation_units[] = { "example.c" },
*cflags[]            = { "-Wall", "-Wextra", "-ggdb", "-O0", "-std=c99", "-pedantic", "-o", "out" },
*libs[]              = { "-lraylib" };


// EXPORT(cc, translation_units, cflags, libs)
