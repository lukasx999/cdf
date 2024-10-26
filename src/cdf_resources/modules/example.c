//bin/true; cc vector.c $0 -o out; ./out; rm out; exit 0

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "./vector.h"
#include "./printing.h"
#include "./colors.h"


typedef struct {
    int a, b, c;
} Foo;


int main(void) {

    vec_Vector vec = vec_new(sizeof(vec_Vector), 5, 2);

    vec_Vector nested = vec_new(sizeof(int), 5, 2);
    VEC_PUSH(&nested, 123);
    VEC_PUSH(&nested, 32);
    VEC_PUSH(&nested, 43);

    vec_push(&vec, &nested);

    for (size_t i = 0; i < vec.size; ++i) {
        vec_Vector *v = (vec_Vector*)vec_get(&vec, i);
        for (size_t j = 0; j < v->size; ++j)
            printf("%d\n", *(int*)vec_get(v, j));
    }



    return EXIT_SUCCESS;

}
