#include <stdio.h>

#define t_assert(fact)                                                         \
    if (!(fact)) {                                                             \
        fprintf(stderr, "Assertion failed: %s at %s:%d in %s()\n", #fact,      \
                __FILE__, __LINE__, __func__);                                 \
        return 1;                                                              \
    }

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
