#pragma once
#include <stddef.h>
#include <stdint.h>

#include "memory.h"
#include "types.h"

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
#define Vec_get(v, T, pos) (T *)(v.data + v.tsize * pos)
