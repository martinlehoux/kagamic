#include "string.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"

// Returns 1 if strings are equal, 0 otherwise
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

Str* Str_copy(Arena *a, char *data, uintptr_t len) {
  Str *str = new(a, Str, 1);
  str->len = len;
  str->data =  new (a, char, len);
  if (str->len)
    memcpy(str->data, data, len);
  return str;
}

void Str_print(Str s) {
    printf("%.*s", (int)s.len, s.data);
}
