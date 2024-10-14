#pragma once
#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>


// TODO: 2D vectors
// TODO: vec_index with complex datatypes (structs, ...)
// TODO: implement vec_get_wrap for wraparound indexing


typedef char vec_blob_t; // pointer arithmetic on voidptrs is undefined => increment by 1 byte via char

typedef struct {
    size_t capacity,
           start_capacity,
           size,
           element_size,
           growth_rate;

    vec_blob_t *blob; // using char* instead of void* for pointer arith

} vec_Vector;

extern void       vec_init          (vec_Vector *v, size_t element_size, size_t start_capacity, size_t growth_rate);
extern void       vec_push          (vec_Vector *v, void *value);
extern void*      vec_get           (vec_Vector *v, size_t index);              // returns `NULL` if index is out of bounds
extern int        vec_set           (vec_Vector *v, size_t index, void *value); // returns -1 on error, else 0
extern int        vec_delete        (vec_Vector *v, size_t index);              // returns -1 on error, else 0
extern int        vec_insert_before (vec_Vector *v, size_t index, void *value);
extern int        vec_insert_after  (vec_Vector *v, size_t index, void *value);
extern void*      vec_pop           (vec_Vector *v);
extern int        vec_extend        (vec_Vector *this, vec_Vector *other);      // merge `other` into `this`
extern void       vec_literal       (vec_Vector *v, ...);                       // push multiple elements in one function call | uses `NULL` as a va terminator
extern void       vec_clear         (vec_Vector *v);
extern ssize_t    vec_index         (vec_Vector *v, void *value);               // returns index of `value` | returns `-1` if `value` was not found
extern vec_Vector vec_copy          (vec_Vector *v, int *err);                  // copies `this` and returns a newly allocated vector | the error code will be put in `err, if `err` is NULL, no error will be reported
extern void       vec_destroy       (vec_Vector *v);                            // using a vector after destroying it will yield undefined behaviour



#endif // _VECTOR_H