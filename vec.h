#include <stddef.h>
#include <stdint.h>

#include "memory.h"

#ifndef VEC_H
#define VEC_H
typedef struct {
  void *data;
  ptrdiff_t len;
  ptrdiff_t cap;
  uintptr_t size;
  int align;
} Vec;
#define Vec_new(a, T, cap) _Vec_new(a, sizeof(T), cap, _Alignof(T))
Vec *_Vec_new(Arena *a, int size, ptrdiff_t cap, int align);
Vec *Vec_push(Arena *a, Vec *v, void *data);
#define Vec_get(v, T, p) (T *)_Vec_get(v, p)
void *_Vec_get(Vec *v, int pos);
#endif
