#pragma once
#include <stddef.h>

#include "types.h"

typedef struct {
    size cap;
    byte *beg;
    byte *end;
} Arena;

Arena Arena_new(size cap);
#define new(a, T, n) (T *)alloc(a, n, sizeof(T), _Alignof(T))
void *alloc(Arena *a, size count, size tsize, size align);
size Arena_get_used(Arena a);
void Arena_reset(Arena *a);
