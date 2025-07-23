#include "string.h"
#include "testing.h"

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
