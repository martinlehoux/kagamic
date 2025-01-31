#include <stddef.h>

#ifndef MEMORY_H
#define MEMORY_H
typedef struct {
  ptrdiff_t cap;
  char *beg;
  char *end;
} Arena;

Arena Arena_new(ptrdiff_t cap);
#define new(a, T, n) (T *)alloc(a, n, sizeof(T), _Alignof(T))
void *alloc(Arena *a, ptrdiff_t count, int size, int align);
int Arena_get_used(Arena a);
#endif
