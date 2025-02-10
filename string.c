#include "string.h"

#include <stdint.h>
#include <string.h>

#include "memory.h"

Str *Str_new(Arena *a, uintptr_t len) {
  Str *s = new (a, Str, 1);
  s->data = new (a, char, len);
  return s;
}

int Str_equals(Str a, Str b) {
  return (a.len == b.len) && (!a.len || !memcmp(a.data, b.data, a.len));
}

uint64_t Str_hash64(Str s) {
  uint64_t h = 0x100;
  for (ptrdiff_t i = 0; i < s.len; i++) {
    h ^= s.data[i] & 255;
    h *= 1111111111111111111;
  }
  return h;
}

Str Str_copy(Arena *a, char *data, uintptr_t len) {
  Str str = {0,len};
  str.data = new (a, char, len);
  if (str.len)
    memcpy(str.data, data, len);
  return str;
}
