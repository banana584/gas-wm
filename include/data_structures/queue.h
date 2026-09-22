#ifndef _GAS_INCLUDE_DATA_STRUCTURES_QUEUE_H
#define _GAS_INCLUDE_DATA_STRUCTURES_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifndef _GAS_QUEUE_START
#define _GAS_QUEUE_START 5
#endif

#ifndef _GAS_QUEUE_THRESHOLD
#define _GAS_QUEUE_THRESHOLD 1.7
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif

#define DECLARE_QUEUE(name, type) \
typedef struct name { \
    type* arr; \
    size_t cap; \
    size_t count; \
    size_t front; \
} name; \
name name##_create(); \
void name##_destroy(name* queue); \
int name##_resize(name* queue, size_t cap); \
int name##_grow(name* queue); \
int name##_shrink(name* queue); \
void name##_push(name* queue, type* val); \
type name##_pop(name* queue); \
type name##_peek(name* queue);

#define IMPL_QUEUE(name, type) \
name name##_create() { \
    name queue = { .cap = _GAS_QUEUE_START }; \
    queue.arr = (type*)malloc(sizeof(type) * queue.cap); \
\
    return queue; \
} \
void name##_destroy(name* queue) { \
    free(queue->arr); \
} \
int name##_resize(name* queue, size_t cap) { \
    cap = MAX(cap, _GAS_QUEUE_START); \
\
    type* ptr = (type*)malloc(sizeof(type) * queue->cap); \
\
    for (size_t i = 0; i < queue->count; i++) { \
        size_t old_idx = (queue->front + i) % queue->cap; \
\
        ptr[i] = queue->arr[old_idx]; \
    } \
\
    free(queue->arr); \
    queue->arr = ptr; \
\
    if (queue->cap < cap) queue->front = 0; \
    queue->cap = cap; \
\
    return 0; \
} \
int name##_grow(name* queue) { \
    size_t count = queue->count * _GAS_QUEUE_THRESHOLD; \
    if (count < queue->cap) return 0; \
\
    return name##_resize(queue, count); \
} \
int name##_shrink(name* queue) { \
    size_t count = queue->cap / _GAS_QUEUE_THRESHOLD; \
    if (count < queue->count) return 0; \
\
    return name##_resize(queue, count); \
} \
void name##_push(name* queue, type* val) { \
    name##_grow(queue); \
\
    size_t idx = (queue->front + queue->count) % queue->cap; \
\
    queue->arr[idx] = *val; \
    queue->count++; \
} \
type name##_pop(name* queue) { \
    type val = queue->arr[queue->front]; \
\
    queue->front = (queue->front + 1) % queue->cap; \
    queue->count--; \
\
    /*name##_shrink(queue);*/ \
\
    return val; \
} \
type name##_peek(name* queue) { \
    return queue->arr[queue->front]; \
}

#endif
