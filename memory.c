#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

Arena Arena_new(ptrdiff_t cap) {
  Arena a = {0};
  a.cap = cap;
  a.beg = malloc(cap);
  a.end = a.beg ? a.beg + cap : 0;
  return a;
}

// Magic to allow optimisation __attribute((malloc, alloc_size(2, 4),
// alloc_align(3)))
void *alloc(Arena *a, ptrdiff_t count, int size, int align) {
  int padding = -(uintptr_t)a->beg & (align - 1);
  assert(count < (a->end - a->beg - padding) / size);
  void *cursor = a->beg + padding;
  a->beg += padding + count * size;
  return memset(cursor, 0, count * size);
}

int Arena_get_used(Arena a) { return a.cap - (a.end - a.beg); }
