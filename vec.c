#include <string.h>

#include "vec.h"

Vec *_Vec_new(Arena *a, int size, ptrdiff_t cap, int align) {
  Vec *v = new (a, Vec, 1);
  v->data = alloc(a, cap, size, align);
  v->size = size;
  v->align = align;
  v->cap = cap;
  v->len = 0;
  return v;
}

Vec *Vec_push(Arena *a, Vec *v, void *data) {
  if (v->len == v->cap) {
    void *cpy = alloc(a, v->cap * 2, v->size, v->align);
    memcpy(cpy, v->data, v->len * v->size);
    v->cap *= 2;
  }
  memcpy(v->data + v->len * v->size, data, v->size);
  v->len++;
  return v;
}

void *_Vec_get(Vec *v, int pos) {
  // printf("get: length=%lu, cap=%lu, pos=%d\n", v->len, v->cap, pos);
  // printf("get: start=%p, asked=%p\n", v->data, v->data + v->size * pos);
  return v->data + v->size * pos;
}
