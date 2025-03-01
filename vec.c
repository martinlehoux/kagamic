#include <string.h>

#include "assert.h"
#include "vec.h"

Vec _Vec_new(Arena *a, int size, ptrdiff_t cap, int align) {
  assert(cap > 0);
  Vec v = {0};
  v.data = alloc(a, cap, size, align);
  v.size = size;
  v.align = align;
  v.cap = cap;
  v.len = 0;
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
