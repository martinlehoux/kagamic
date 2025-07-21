#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "types.h"

Arena Arena_new(size cap) {
    Arena a = {0};
    a.cap = cap;
    a.beg = malloc(cap);
    a.end = a.beg ? a.beg + cap : 0;
    return a;
}

// Magic to allow optimisation __attribute((malloc, alloc_size(2, 4),
// alloc_align(3)))
void *alloc(Arena *a, size count, size tsize, int align) {
    int padding = -(uptr)a->beg & (align - 1);
    assert(count < (a->end - a->beg - padding) / tsize);
    void *cursor = a->beg + padding;
    a->beg += padding + count * tsize;
    return memset(cursor, 0, count * tsize);
}

int Arena_get_used(Arena a) { return a.cap - (a.end - a.beg); }
