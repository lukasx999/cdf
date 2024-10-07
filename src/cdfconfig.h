#define CDF_HEADER
#include "cdf.c"
SETUP()



static const char
*cc                  = "gcc",
*translation_units[] = { "project.c" },
*cflags[]            = { "-Wall", "-Wextra", "-ggdb", "-O0", "-std=c99", "-pedantic", "-o", "out" },
*libs[]              = { NULL };
