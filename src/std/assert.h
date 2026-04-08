#pragma once

#include <stdio.h>
#include <stdlib.h>

#define assert(condition, ...)                                                 \
    do {                                                                       \
        if (!(condition)) {                                                    \
            fprintf(stderr, "Assertion failed: %s:%d in %s(): ", __FILE__,     \
                    __LINE__, __func__);                                       \
            fprintf(stderr, __VA_ARGS__);                                      \
            fprintf(stderr, "\n");                                             \
            abort();                                                           \
        }                                                                      \
    } while (0)
