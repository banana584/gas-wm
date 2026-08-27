#ifndef _GAS_INCLUDE_DATA_STRUCTURES_RING_H
#define _GAS_INCLUDE_DATA_STRUCTURES_RING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define DECLARE_RING(name, type) \
typedef struct name { \
    type* arr; \
    size_t count; \
    size_t head; \
    size_t tail; \
} name; \
name name##_create(const size_t cap); \
void name##_destroy(name* ring); \
bool name##_push(name* ring, const type value); \
type name##_pop(name* ring); \

#define IMPL_RING(name, type)

#endif
