#pragma once
#ifndef _CDFSTRING_H
#define _CDFSTRING_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct {
    size_t _length;
    size_t _capacity;
    char *str;
} String;


extern String string_new(void);
extern String string_from(const char *str);

extern void string_destroy(String *s);
extern void string_append_char(String *s, char c);
extern void string_concat(String *this, const String *other); // concatenates other to this









#endif // _CDFSTRING_H
