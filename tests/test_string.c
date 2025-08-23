#include "../include/string.h"
#include "../include/testing.h"
#include "../include/types.h"
#include <stdio.h>

i32 test_string_literal_equality() {
    t_assert(Str_equals(S("Bonjour"), S("Bonjour")));
    return 0;
}

i32 test_string_copy() {
    Arena perm = Arena_new(128e3);
    Str src = S("Hello");

    Str cpy = Str_copy(&perm, src.data, src.len);

    t_assert(Str_equals(src, cpy));
    t_assert_equal(Arena_get_used(perm), 5);

    return 0;
}

i32 test_fast_string() {
    byte_32 quote_mask = _mm256_set1_epi8('"');
    byte *s = malloc(32 * sizeof(byte));
    s = "abcdefghijklmnopqrstuvwxyzabcdef";

    byte_32 chunk = _mm256_loadu_si256((byte_32 *)s);
    byte_32 cmp = _mm256_cmpeq_epi8(chunk, quote_mask);
    i32 mask = _mm256_movemask_epi8(cmp);
    t_assert(_mm_popcnt_u32(mask) == 0);

    s = "abcdefghijklm\"nopqrstuvwxyzabcde";

    chunk = _mm256_loadu_si256((byte_32 *)s);
    cmp = _mm256_cmpeq_epi8(chunk, quote_mask);
    mask = _mm256_movemask_epi8(cmp);
    t_assert(_mm_popcnt_u32(mask) == 1);

    return 0;
}
