#pragma once
#ifndef _PRINTING_H
#define _PRINTING_H

#include <errno.h>
#include <stdio.h>

#define ERROR(message) { \
    perror(message);     \
    exit(1);             \
}

#endif // _PRINTING_H
