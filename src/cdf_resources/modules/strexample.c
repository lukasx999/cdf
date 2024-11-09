//bin/true; cc cdfstring.c $0 -ggdb -o out && ./out; exit 0

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "./cdfstring.h"


int main(void) {

    String s = string_from("foo");
    String a = string_from("bar");
    string_append_char(&s, 'X');

    string_concat(&s, &a);


    printf("%s\n", s.str);


    string_destroy(&s);



    return EXIT_SUCCESS;

}
