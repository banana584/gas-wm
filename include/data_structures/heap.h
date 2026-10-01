#ifndef _GAS_INCLUDE_DATA_STRUCTURES_HEAP_H
#define _GAS_INCLUDE_DATA_STRUCTURES_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define GAS_HEAP_PARENT(i) (((i) - 1) / 2)
#define GAS_HEAP_LEFT(i) (2 * (i) + 1)
#define GAS_HEAP_RIGHT(i) (2 * (i) + 2)

#ifndef _GAS_HEAP_START
#define _GAS_HEAP_START 5
#endif

#ifndef _GAS_HEAP_THRESHOLD
#define _GAS_HEAP_THRESHOLD 2
#endif

typedef bool (*gas_heap_less_than)(void* left, void* right);
typedef bool (*gas_heap_greater_than)(void* left, void* right);

#define DECLARE_HEAP(name, type) \
typedef struct name { \
    type* arr; \
    size_t count; \
    size_t cap; \
    gas_heap_less_than less; \
    gas_heap_greater_than greater; \
} name; \
name name##_create(); \
void name##_destroy(name* heap); \
int name##_resize(name* heap, size_t cap); \
int name##_grow(name* heap); \
void name##_insert(name* heap, type* val); \
type name##_peek(name* heap); \
type name##_pop(name* heap);

#define IMPL_HEAP(name, type, max, func_less, func_greater) \
name name##_create() { \
    name heap = { .cap = _GAS_HEAP_START }; \
\
    if (max) { \
        heap.less = func_greater; \
        heap.greater = func_less; \
    } else { \
        heap.less = func_less; \
        heap.greater = func_greater; \
    } \
\
    heap.arr = (type*)malloc(sizeof(type) * heap.cap); \
\
    return heap; \
} \
void name##_destroy(name* heap) { \
    free(heap->arr); \
} \
int name##_resize(name* heap, size_t cap) { \
    type* ptr = (type*)realloc(heap->arr, sizeof(type) * heap->cap); \
    if (!ptr) { \
        perror("realloc"); \
        return -1; \
    } \
\
    heap->arr = ptr; \
    heap->cap = cap; \
\
    return 0; \
} \
int name##_grow(name* heap) { \
    size_t cap = heap->count * _GAS_HEAP_THRESHOLD; \
    if (cap < heap->cap) return 0; \
    \
    return name##_resize(heap, cap); \
} \
void name##_insert(name *heap, type *val) { \
    name##_grow(heap); \
\
    size_t idx = heap->count++; \
\
    while (idx > 0) { \
        size_t parent = GAS_HEAP_PARENT(idx); \
\
        if (!heap->less(val, &heap->arr[parent])) { \
            break; \
        } \
        heap->arr[idx] = heap->arr[parent]; \
        idx = parent; \
    } \
\
    heap->arr[idx] = *val; \
} \
type name##_peek(name* heap) { \
    return heap->arr[0]; \
} \
type name##_pop(name *heap) { \
    type val = heap->arr[0]; \
    size_t count = --heap->count; \
\
    if (count == 0) { \
        return val; \
    } \
\
    type item = heap->arr[count]; \
    size_t idx = 0; \
\
    while (1) { \
        size_t left = GAS_HEAP_LEFT(idx); \
        if (left >= count) { \
            break; \
        } \
        size_t right = GAS_HEAP_RIGHT(idx); \
        size_t child = left; \
\
        if (right < count && heap->less(&heap->arr[right], &heap->arr[left])) { \
            child = right; \
        } \
\
        if (!heap->less(&heap->arr[child], &item)) { \
            break; \
        } \
        heap->arr[idx] = heap->arr[child]; \
        idx = child; \
    } \
\
    heap->arr[idx] = item; \
    return val; \
}


#endif