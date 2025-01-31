#include <stddef.h>
#include <stdint.h>

#include "memory.h"

#ifndef STRING_H
#define STRING_H
#define S(s) (Str){s, sizeof(s) - 1}
typedef struct {
  char *data;
  ptrdiff_t len;
} Str;
Str *Str_new(Arena *a, uintptr_t len);
int Str_equals(Str a, Str b);
uint64_t Str_hash64(Str s);
Str Str_copy(Arena *a, char *data, uintptr_t len);
#endif
