#include <stddef.h>
#include <stdint.h>

#include "memory.h"
#include "types.h"

#ifndef VEC_H
#define VEC_H
typedef struct {
    void *data;
    size len;
    size cap;
    size tsize;
    size align;
} Vec;
#define Vec_new(a, T, cap) _Vec_new(a, sizeof(T), cap, _Alignof(T))
Vec _Vec_new(Arena *a, size tsize, size cap, size align);
void Vec_push(Arena *a, Vec *v, void *data);
#define Vec_get(v, T, p) (T *)_Vec_get(v, p)
void *_Vec_get(Vec v, size pos);
#endif
