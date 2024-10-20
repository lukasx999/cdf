//bin/true; cc vector.c $0 -o out; ./out; rm out; exit 0

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "./vector.h"
#include "./smart_printing.h"
#include "./colors.h"

void call(void *arg) {
    // printf("value: %d\n", *(int*)arg);
    PRINT(*(int*)arg);
}




int main(void) {

    vec_Vector vec = vec_new(sizeof(int), 5, 2);
    VEC_PUSH(&vec, 123);

    vec_foreach(&vec, call);



    return EXIT_SUCCESS;

}
