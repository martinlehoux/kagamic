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
                (int)a, #a, (int)b, #b, __FILE__, __LINE__, __func__);         \
        return 1;                                                              \
    }
