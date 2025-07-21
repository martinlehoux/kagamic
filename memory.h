#include <stddef.h>

#ifndef MEMORY_H
#include "types.h"
#define MEMORY_H
typedef struct {
    size cap;
    byte *beg;
    byte *end;
} Arena;

Arena Arena_new(size cap);
#define new(a, T, n) (T *)alloc(a, n, sizeof(T), _Alignof(T))
void *alloc(Arena *a, size count, size size, int align);
int Arena_get_used(Arena a);
#endif
