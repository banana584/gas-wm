#ifndef _GAS_INCLUDE_DATA_STRUCTURES_SLOT_MAP_H
#define _GAS_INCLUDE_DATA_STRUCTURES_SLOT_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "vector.h"

#ifndef _GAS_SLOT_MAP_START
#define _GAS_SLOT_MAP_START 5
#endif

#ifndef _GAS_SLOT_MAP_THRESHOLD
#define _GAS_SLOT_MAP_THRESHOLD 2
#endif

#define DECLARE_SLOT_MAP(name, type) \
DECLARE_VECTOR(name##_free, size_t) \
typedef struct name { \
    type* arr; \
    size_t count; \
    size_t cap; \
    size_t right; \
    name##_free free; \
} name; \
name name##_create(); \
void name##_destroy(name* slots); \
int name##_resize(name* slots, size_t cap); \
int name##_grow(name* slots); \
int name##_shrink(name* slots); \
type name##_get(name* slots, size_t idx); \
void name##_set(name* slots, size_t idx, type* val); \
size_t name##_push(name* slots, type* val); \
type name##_pop(name* slots, size_t idx);

#define IMPL_SLOT_MAP(name, type) \
IMPL_VECTOR(name##_free, size_t) \
name name##_create() { \
    name slots = { .cap = _GAS_SLOT_MAP_START }; \
    slots.free = name##_free_create(); \
 \
    slots.arr = (type*)malloc(sizeof(type) * slots.cap); \
    memset(slots.arr, 0, sizeof(type) * slots.cap); \
\
    return slots; \
} \
void name##_destroy(name* slots) { \
    free(slots->arr); \
    name##_free_destroy(&slots->free); \
} \
int name##_resize(name* slots, size_t cap) { \
    type* ptr = (type*)realloc(slots->arr, sizeof(type) * cap); \
\
    if (!ptr) { \
        perror("slots realloc"); \
        return -1; \
    } \
\
    slots->arr = ptr; \
    slots->cap = cap; \
    return 0; \
} \
int name##_grow(name* slots) { \
    size_t count = slots->count * _GAS_SLOT_MAP_THRESHOLD; \
    if (count < slots->cap) return 0; \
\
    return name##_resize(slots, count); \
} \
int name##_shrink(name* slots) { \
    size_t count = slots->cap / _GAS_SLOT_MAP_THRESHOLD; \
    if (count < slots->count || (slots->cap - slots->right) < count) return 0; \
\
    return name##_resize(slots, count); \
} \
type name##_get(name* slots, size_t idx) { \
    return slots->arr[idx]; \
} \
void name##_set(name* slots, size_t idx, type* val) { \
    slots->arr[idx] = *val; \
} \
size_t name##_push(name* slots, type* val) { \
    name##_grow(slots); \
\
    size_t idx; \
\
    if (slots->free.count > 0) { \
        do { \
            idx = name##_free_pop(&slots->free, slots->free.count); \
        } while (idx > slots->count && slots->free.count > 0); \
\
        if (slots->free.count == 0) { \
            goto zero_left; \
        } \
    } else { \
        zero_left: \
        idx = slots->right++; \
    } \
\
    slots->arr[idx] = *val; \
    slots->count++; \
\
    return idx; \
} \
type name##_pop(name* slots, size_t idx) { \
    name##_free_push(&slots->free, &idx); \
\
    return slots->arr[idx]; \
} \

#endif
