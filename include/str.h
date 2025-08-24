#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/memory.h"
#include "../include/types.h"

typedef struct {
    byte *data;
    size len;
} Str;
u64 Str_hash64(Str s);
Str Str_copy(Arena *a, byte *data, size len);
i32 Str_fprint(FILE *w, Str s);

typedef struct {
    byte *data;
    size len;
} StrView;
StrView Str_view(byte *data, size len);
Str StrView_copy(Arena *a, StrView s);

// Macros working with both types
// Returns 1 if strings are equal, 0 otherwise
#define Str_equals(a, b)                                                       \
    ((a).len == (b).len) && (!(a).len || !memcmp((a).data, (b).data, (a).len))
#define Str_fprint(w, s) fprintf(w, "%.*s", (i32)(s).len, (s).data)
#define S(s)                                                                   \
    (Str) { s, lengthof(s) }
