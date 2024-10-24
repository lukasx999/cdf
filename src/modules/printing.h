#pragma once
#ifndef _PRINTING_H
#define _PRINTING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define _DEBUG(var, fmt) fprintf(stderr, "%s=" fmt "\n", (#var), (var))

#define PRINT(var) _Generic((var),         \
             int     : _DEBUG(var, "%d" ), \
             size_t  : _DEBUG(var, "%lu"), \
             bool    : _DEBUG(var, "%b" ), \
             char    : _DEBUG(var, "%c" ), \
             char*   : _DEBUG(var, "%s" ), \
             void*   : _DEBUG(var, "%p" ), \
             default : assert(!"unknown type"));

#endif // _PRINTING_H
