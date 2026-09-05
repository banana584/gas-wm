#ifndef _GAS_INCLUDE_DATA_STRUCTURES_HASHMAP_H
#define _GAS_INCLUDE_DATA_STRUCTURES_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifndef _GAS_HASHMAP_START
#define _GAS_HASHMAP_START 2
#endif

#ifndef _GAS_HASHMAP_THRESHOLD
#define _GAS_HASHMAP_THRESHOLD 1.7
#endif

uint64_t hash(const char* str);

#define DECLARE_HASHMAP(name, type) \
typedef struct name##_entry { \
    char* key; \
    type value; \
    struct name##_entry* next; \
} name##_entry; \
typedef struct name { \
    size_t count; \
    size_t cap; \
    name##_entry* buckets; \
} name; \
\
name name##_create(); \
void name##_destroy(name* map); \
bool name##_resize(name* map, const size_t count); \
bool name##_grow(name* map); \
bool name##_shrink(name* map); \
void name##_set(name* map, const char* key, type* val); \
type name##_get(name* map, const char* key, bool* found);

#define IMPL_HASHMAP(name, type) \
name name##_create() { \
    name map = { .cap = _GAS_HASHMAP_START }; \
\
    map.buckets = (name##_entry*)malloc(sizeof(name##_entry) * map.cap); \
    if (!map.buckets) { \
        perror("malloc"); \
        map.cap = 0; \
    } \
\
    return map; \
} \
void name##_destroy(name* map) { \
    for (size_t i = 0; i < map->cap; i++) { \
        name##_entry entry = map->buckets[i]; \
\
        if (entry.key) { \
            free(entry.key); \
        } \
\
        name##_entry* current = entry.next; \
        while (current) { \
            name##_entry* tmp = current->next; \
            free(current->key); \
            free(current); \
            current = tmp; \
        } \
    } \
\
    free(map->buckets); \
    map->buckets = NULL; \
} \
bool name##_resize(name* map, const size_t count) { \
    name##_entry* ptr = (name##_entry*)realloc(map->buckets, count * sizeof(name##_entry)); \
    if (!ptr) { \
        perror("realloc"); \
        return false; \
    } \
\
    map->count = count; \
    map->buckets = ptr; \
    return true; \
} \
bool name##_grow(name* map) { \
    const size_t count = (map->count + 1) * _GAS_HASHMAP_THRESHOLD; \
    if (count < map->cap) { \
        return true; \
    } \
\
    return name##_resize(map, count); \
} \
bool name##_shrink(name* map) { \
    const size_t cap = map->cap / _GAS_HASHMAP_THRESHOLD; \
    if (map->count < cap) { \
        return true; \
    } \
\
    return name##_resize(map, cap); \
} \
void name##_set(name* map, const char* key, type* val) { \
    name##_grow(map); \
\
    uint64_t idx = hash(key) % map->cap; \
\
    name##_entry* current = &map->buckets[idx]; \
\
    while (current) { \
        if (!current->key) { \
            current->key = strdup(key); \
            current->value = *val; \
            return; \
        } \
\
        if (strcmp(current->key, key) == 0) { \
            current->value = *val; \
            return; \
        } \
\
        current = current->next; \
    } \
} \
type name##_get(name* map, const char* key, bool* found) {\
    uint64_t idx = hash(key) % map->cap; \
\
    name##_entry* current = &map->buckets[idx]; \
\
    while (current) { \
        if (!current->key) { \
            *found = false; \
            return (type)0; \
        } \
\
        if (strcmp(current->key, key) == 0) { \
            *found = true; \
            return current->value; \
        } \
\
        current = current->next; \
    } \
\
    *found = false; \
    return (type)0; \
} \

#endif
