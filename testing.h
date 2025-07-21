#include <stdio.h>

#define t_assert(fact)                                                         \
  if (!(fact)) {                                                               \
    fprintf(stderr, "Assertion failed: %s at %s:%d in %s()\n",  #fact, __FILE__, __LINE__, __func__); \
    return 1;                                                                  \
  }
