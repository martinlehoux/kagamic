#include <string.h>

#include "assert.h"
#include "types.h"
#include "vec.h"

Vec _Vec_new(Arena *a, size tsize, size cap, int align) {
    assert(cap > 0);
    Vec v = {.size = tsize, .cap = cap, .len = 0, .align = align, .data = 0};
    v.data = alloc(a, cap, tsize, align);
    return v;
}

void Vec_push(Arena *a, Vec *v, void *data) {
    if (v->len == v->cap) {
        void *cpy = alloc(a, v->cap * 2, v->size, v->align);
        memcpy(cpy, v->data, v->len * v->size);
        v->cap *= 2;
    }
    memcpy(v->data + v->len * v->size, data, v->size);
    v->len++;
}

void *_Vec_get(Vec *v, int pos) { return v->data + v->size * pos; }
