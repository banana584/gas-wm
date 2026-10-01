#ifndef _GAS_INCLUDE_DATA_STRUCTURES_VECTOR_H
#define _GAS_INCLUDE_DATA_STRUCTURES_VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifndef _GAS_VECTOR_START
#define _GAS_VECTOR_START 5
#endif

#ifndef _GAS_VECTOR_THRESHOLD
#define _GAS_VECTOR_THRESHOLD 2
#endif

#define DECLARE_VECTOR(name, type) \
typedef struct name { \
    type* arr; \
    size_t count; \
    size_t cap; \
} name; \
\
name name##_create(); \
void name##_destroy(name* vec); \
void name##_reset(name* vec); \
int name##_resize(name* vec, size_t cap); \
int name##_grow(name* vec); \
int name##_shrink(name* vec); \
type name##_get(name* vec, size_t idx); \
void name##_set(name* vec, size_t idx, type* val); \
size_t name##_push(name* vec, type* val); \
type name##_pop(name* vec, size_t idx); \

#define IMPL_VECTOR(name, type) \
name name##_create() { \
    name vec = {0}; \
    vec.arr = (type*)malloc(sizeof(type) * _GAS_VECTOR_START); \
    vec.cap = _GAS_VECTOR_START; \
\
    return vec; \
} \
void name##_destroy(name* vec) { \
    free(vec->arr); \
} \
void name##_reset(name* vec) { \
    vec->count = 0; \
    name##_resize(vec, _GAS_VECTOR_START); \
    memset(vec->arr, 0, sizeof(type) * vec->cap); \
} \
int name##_resize(name* vec, size_t cap) { \
    type* new = realloc(vec->arr, sizeof(type) * cap); \
\
    if (!new) { \
        perror("vector realloc"); \
        return -1; \
    } \
\
    vec->arr = new; \
    vec->cap = cap; \
    return 0; \
} \
int name##_grow(name* vec) { \
    size_t count = vec->count * _GAS_VECTOR_THRESHOLD; \
    if (count < vec->cap) return 0; \
\
    return name##_resize(vec, count); \
} \
int name##_shrink(name* vec) { \
    size_t count = vec->cap / _GAS_VECTOR_THRESHOLD; \
    if (count < vec->count) return 0; \
\
    return name##_resize(vec, count); \
} \
type name##_get(name* vec, size_t idx) { \
    return vec->arr[idx]; \
} \
void name##_set(name* vec, size_t idx, type* val) { \
    vec->arr[idx] = *val; \
} \
size_t name##_push(name* vec, type* val) { \
    name##_grow(vec); \
\
    size_t idx = vec->count++; \
    vec->arr[idx] = *val; \
\
    return idx; \
} \
type name##_pop(name* vec, size_t idx) { \
    type val = vec->arr[idx]; \
\
    if (vec->count - idx > 0) memmove(vec->arr + idx, vec->arr + idx + 1, (vec->count - idx) * sizeof(type)); \
\
    vec->count--; \
    /*name##_shrink(vec);*/ \
\
    return val; \
}

#endif