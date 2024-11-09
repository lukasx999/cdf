#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./cdfstring.h"



String
string_new(void) {

    String s = {
        ._length   = 0,
        ._capacity = 5,
        .str       = NULL,
    };
    s.str = calloc(s._capacity, sizeof(char));
    return s;

}


String
string_from(const char *str) {

    String s = {
        ._length   = strlen(str),
        ._capacity = strlen(str)+1,
        .str       = NULL,
    };
    s.str = calloc(s._capacity, sizeof(char));
    strncpy(s.str, str, strlen(str));
    return s;

}

void
string_destroy(String *s) {
    s->_capacity = 0;
    s->_length = 0;
    free(s->str);
}


void
string_append_char(String *s, char c) {

    if (s->_length == s->_capacity-1) {
        s->_capacity *= 2;
        s->str = realloc(s->str, s->_capacity * sizeof(char));
    }

    s->str[s->_length++] = c;

}

void
string_concat(String *this, const String *other) {

    this->_capacity += other->_length;
    this->str = realloc(this->str, this->_capacity * sizeof(char));

    char *offset = this->str + this->_length;
    strncpy(offset, other->str, other->_length * sizeof(char));

}



