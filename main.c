/*
Inspirations:
- https://nullprogram.com/
*/
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "memory.h"
#include "string.h"
#include "vec.h"

#define sizeof(x) (ptrdiff_t)sizeof(x)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

typedef struct {
  int age;
} User;

int main() { return 0; }
