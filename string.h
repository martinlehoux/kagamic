#include <stddef.h>
#include <stdint.h>

#include "memory.h"
#include "types.h"

#ifndef STRING_H
#define STRING_H
#define S(s) (Str){s, sizeof(s) - 1}
typedef struct {
  byte *data;
  size len;
} Str;
int Str_equals(Str a, Str b);
uint64_t Str_hash64(Str s);
Str* Str_copy(Arena *a, byte *data, size len);
void Str_print(Str s);
#endif
