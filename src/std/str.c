#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "str.h"

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

StrView Str_view(byte *data, size len) {
    StrView str = {0};
    str.len = len;
    str.data = data;
    return str;
}

Str StrView_copy(Arena *a, StrView s) { return Str_copy(a, s.data, s.len); }
