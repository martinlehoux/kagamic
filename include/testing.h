#pragma once
#include <stdio.h>
#include <string.h>

#define t_assert_equal(a, b)                                                   \
    if (!(a == b)) {                                                           \
        fprintf(stderr,                                                        \
                "Assertion failed: %d (%s) != %d (%s) at %s:%d in %s()\n",     \
                (i32)a, #a, (i32)b, #b, __FILE__, __LINE__, __func__);         \
        return 1;                                                              \
    }

#define t_assert_fequal(a, b)                                                  \
    if ((a - b) > 1e-6 || (b - a) > 1e-6) {                                    \
        fprintf(stderr,                                                        \
                "Assertion failed: %f (%s) != %f (%s) at %s:%d in %s()\n", a,  \
                #a, b, #b, __FILE__, __LINE__, __func__);                      \
        return 1;                                                              \
    }

#define t_assert_strequal(a, b)                                                \
    if ((a.len != b.len) || (a.len && memcmp(a.data, b.data, a.len))) {        \
        fprintf(stderr,                                                        \
                "Assertion failed: %.*s (%s) != %.*s (%s) at %s:%d in %s()\n", \
                (i32)a.len, a.data, #a, (i32)b.len, b.data, #b, __FILE__,      \
                __LINE__, __func__);                                           \
        return 1;                                                              \
    }
