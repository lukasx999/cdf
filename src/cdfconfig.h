#define CDF_HEADER
#include "cdf.c"
SETUP()

// #define EXPORT(...) assert(true)

static const char
*cc                  = "gcc",
*translation_units[] = { "example.c" },
*cflags[]            = { "-Wall", "-Wextra", "-ggdb", "-O0", "-std=c99", "-pedantic", "-o", "out" },
*libs[]              = { "-lraylib" };


enum {
    MOD_SMARTPRINTING,
    MOD_VECTOR,
    MOD_HASHTABLE,
};


// toggle cdf modules
static bool modules[]   = {
    [MOD_SMARTPRINTING] = 1,
    [MOD_VECTOR]        = 1,
    [MOD_HASHTABLE]     = 0,
};

// EXPORT(cc, translation_units, cflags, libs)
