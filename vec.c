#include <string.h>

#include "assert.h"
#include "types.h"
#include "vec.h"

Vec *_Vec_new(Arena *a, size tsize, size cap, size align) {
    assert(cap > 0);
    Vec *v = new(a, Vec, 1);
    v->tsize = tsize;
    v->cap = cap;
    v->align = align;
    v->len = 0;
    v->data = alloc(a, cap, tsize, align);
    return v;
}

void Vec_push(Arena *a, Vec *v, void *data) {
    if (v->len == v->cap) {
        void *cpy = alloc(a, v->cap * 2, v->tsize, v->align);
        memcpy(cpy, v->data, v->len * v->tsize);
        v->cap *= 2;
    }
    memcpy(v->data + v->len * v->tsize, data, v->tsize);
    v->len++;
}

void *_Vec_get(Vec *v, size pos) { return v->data + v->tsize * pos; }
