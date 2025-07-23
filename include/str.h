#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/memory.h"
#include "../include/types.h"

#define S(s)                                                                   \
    (Str) { s, lengthof(s) }
typedef struct {
    byte *data;
    size len;
} Str;
i32 Str_equals(Str a, Str b);
u64 Str_hash64(Str s);
Str Str_copy(Arena *a, byte *data, size len);
i32 Str_fprint(FILE *w, Str s);
