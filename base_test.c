#include "include/types.h"

#define test(func)                                                             \
    switch (func()) {                                                          \
    case 0:                                                                    \
        printf("✅ %s\n", #func);                                              \
        success++;                                                             \
        break;                                                                 \
    default:                                                                   \
        failures++;                                                            \
        printf("🚫 %s\n", #func);                                              \
    }

i32 success;
i32 failures;
