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
void *alloc(Arena *a, size count, size tsize, size align) {
    size padding = -(uptr)a->beg & (align - 1);
    byte *beg = a->beg + padding;
    size alloc_size = count * tsize;
    assert(beg + alloc_size < a->end);
    a->beg = beg + alloc_size;
    return memset(beg, 0, count * tsize);
}

void Arena_reset(Arena *a) { a->beg = a->end - a->cap; }

size Arena_get_used(Arena a) { return a.cap - (a.end - a.beg); }
