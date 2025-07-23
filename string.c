#include "string.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"

// Returns 1 if strings are equal, 0 otherwise
i32 Str_equals(Str a, Str b) {
    return (a.len == b.len) && (!a.len || !memcmp(a.data, b.data, a.len));
}

u64 Str_hash64(Str s) {
    u64 h = 0x100;
    for (size i = 0; i < s.len; i++) {
        h ^= s.data[i] & 255;
        h *= 1111111111111111111;
    }
    return h;
}

Str Str_copy(Arena *a, byte *data, size len) {
    Str str = {0};
    str.len = len;
    str.data = new(a, byte, len);
    if (str.len) memcpy(str.data, data, len);
    return str;
}

i32 Str_fprint(FILE *w, Str s) {
    return fprintf(w, "%.*s", (i32)s.len, s.data);
}
