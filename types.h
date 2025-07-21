#include <stddef.h>
#include <stdint.h>

typedef ptrdiff_t size;
typedef char byte;
typedef uintptr_t uptr;

#define sizeof(x) (size)sizeof(x)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)
