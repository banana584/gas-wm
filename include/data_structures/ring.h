#ifndef _GAS_INCLUDE_DATA_STRUCTURES_RING_H
#define _GAS_INCLUDE_DATA_STRUCTURES_RING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define DECLARE_RING(name, type) \
typedef struct name { \
    type* arr; \
    size_t count; \
    size_t cap; \
    size_t read; \
    size_t write; \
} name; \
name name##_create(const size_t cap); \
void name##_destroy(name* ring); \
bool name##_push(name* ring, type* value); \
type name##_peek(name* ring); \
type name##_pop(name* ring); \

#define IMPL_RING(name, type, overwrite) \
name name##_create(const size_t cap) { \
    name ring = { .cap = cap }; \
\
    ring.arr = (type*)malloc(sizeof(type) * cap); \
    if (!ring.arr) { \
        perror("malloc"); \
        ring.count = 0; \
    } \
    memset(ring.arr, 0, sizeof(type) * cap); \
\
    return ring; \
} \
void name##_destroy(name* ring) { \
    free(ring->arr); \
    ring->arr = NULL; \
} \
bool name##_push(name* ring, type* value) { \
    if (!overwrite && ring->write != 0) return false; \
\
    ring->arr[ring->write] = *value; \
    ring->write = (ring->write + 1) % ring->cap; \
    ring->count++; \
\
    return true; \
} \
type name##_peek(name* ring) { \
    return ring->arr[ring->read]; \
} \
type name##_pop(name* ring) { \
    type val = ring->arr[ring->read]; \
    ring->read = (ring->read + 1) % ring->cap; \
    ring->count--; \
\
    return val; \
} \

#endif