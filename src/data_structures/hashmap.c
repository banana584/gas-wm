#include "../../include/data_structures/hashmap.h"

uint64_t hash(const char* str) {
    uint64_t h = 5381;
    int c;

    while ((c = *str++)) {
        h = ((h << 5) + h) + c;
    }

    return h;
}
