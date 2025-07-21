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

int success;
int failures;
