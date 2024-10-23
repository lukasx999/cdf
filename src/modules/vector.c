#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>

#include "vector.h"



void
vec_init(vec_Vector *v,
         size_t      element_size,
         size_t      start_capacity,
         size_t      growth_rate) {

    assert(v != NULL);
    assert(growth_rate != 0);
    assert(element_size != 0);

    v->size         = 0;
    v->_capacity     = v->_start_capacity = start_capacity;
    v->element_size = element_size;
    v->_growth_rate  = growth_rate;
    v->blob         = calloc(v->_capacity, v->element_size);
    assert(v->blob != NULL);

}


vec_Vector
vec_new(size_t element_size, size_t start_capacity, size_t growth_rate) {

    assert(growth_rate != 0);
    assert(element_size != 0);

    vec_Vector new = { 0 };
    vec_init(&new, element_size, start_capacity, growth_rate);
    return new;

}



void
vec_push(vec_Vector *v, void *value) {

    assert(v != NULL);
    assert(value != NULL);

    if (v->size+1 == v->_capacity) {
        v->_capacity *= v->_growth_rate;
        v->blob = realloc(v->blob, v->_capacity * v->element_size);
        assert(v->blob != NULL);
    }

    void *dest = v->blob + v->size * v->element_size;
    memcpy(dest, value, v->element_size);
    ++v->size;

}

void
vec_multipush(vec_Vector *v, ...) {
    assert(v != NULL);

    va_list args;
    va_start(args, v);

    for (;;) {
        void *value = va_arg(args, void*);
        if (value == NULL) break;
        vec_push(v, value);
    }

    va_end(args);
}

void*
vec_get(vec_Vector *v, size_t index) {

    assert(v != NULL);

    // negative indexes will yield a very large number, as ´size_t´ is unsigned
    if (index > v->size)
        return NULL;

    return v->blob + index * v->element_size;

}

int
vec_set(vec_Vector *v, size_t index, void *value) {

    assert(value != NULL);
    assert(v != NULL);

    if (index > v->size)
        return -1;

    void *dest = v->blob + index * v->element_size;
    memcpy(dest, value, v->element_size);

    return 0;

}

int
vec_delete(vec_Vector *v, size_t index) {

    assert(v != NULL);

    if (index > v->size)
        return -1;

    void *dest      = v->blob + index     * v->element_size;
    const void *src = v->blob + (index+1) * v->element_size;
    size_t n        = (v->size - index+1) * v->element_size;

    memmove(dest, src, n);
    --v->size;

    return 0;

}

int
vec_insert_before(vec_Vector *v, size_t index, void *value) {

    assert(v != NULL);
    assert(value != NULL);

    if (index > v->size)
        return -1;

    ++v->_capacity;
    v->blob = realloc(v->blob, v->_capacity * v->element_size);
    assert(v->blob != NULL);

    void *dest      = v->blob + (index+1) * v->element_size;
    const void *src = v->blob + index     * v->element_size;
    size_t n        = (v->size - index+1) * v->element_size;

    memmove(dest, src, n);
    ++v->size;

    return vec_set(v, index, value);

}

int
vec_insert_after(vec_Vector *v, size_t index, void *value) {
    assert(v != NULL);

    if (index > v->size)
        return -1;

    ++v->_capacity;
    v->blob = realloc(v->blob, v->_capacity * v->element_size);
    assert(v->blob != NULL);

    void *dest      = v->blob + (index+2) * v->element_size;
    const void *src = v->blob + (index+1) * v->element_size;
    size_t n        = (v->size - index+1) * v->element_size;

    memmove(dest, src, n);
    ++v->size;

    return vec_set(v, index+1, value);

}

void*
vec_pop(vec_Vector *v) {
    assert(v != NULL);

    return v->blob + --v->size * v->element_size;

}

int
vec_extend(vec_Vector *this, vec_Vector *other) {
    assert(this != NULL);
    assert(other != NULL);
    assert(this->element_size == other->element_size); // both vectors must have the same type

    for (size_t i=0; i<other->size; ++i) {
        void *item = vec_get(other, i);
        if (item == NULL) return -1;
        vec_push(this, item);
    }
    return 0;

}

ssize_t
vec_index(vec_Vector *v, void *value) {
    assert(v != NULL);
    assert(value != NULL);

    for (size_t i = 0; i < v->size; ++i) {
        vec_blob_t *item = vec_get(v, i);
        if (item == NULL) return -1;

        if (!memcmp(item, value, v->element_size))
            return i;

    }
    return -1;

}

vec_Vector
vec_copy(vec_Vector *v, int *err) {

    vec_Vector new;
    vec_init(&new, v->element_size, v->_start_capacity, v->_growth_rate);
    int s = vec_extend(&new, v);
    if (err != NULL) *err = s;
    return new;

}

void
vec_clear(vec_Vector *v) {
    assert(v != NULL);

    vec_destroy(v);
    vec_init(v, v->element_size, v->_start_capacity, v->_growth_rate);
}

void
vec_foreach(vec_Vector *v, vec_callback_t callback) {

    for (size_t i = 0; i < v->size; ++i)
        callback(vec_get(v, i));

}

void
vec_destroy(vec_Vector *v) {
    assert(v != NULL);

    free(v->blob);
}
