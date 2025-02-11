/*
Inspirations:
- https://nullprogram.com/
*/
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "json.h"
#include "memory.h"
#include "string.h"
#include "vec.h"

#define sizeof(x) (ptrdiff_t)sizeof(x)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

typedef struct {
  int age;
} User;

int main() {
  Arena perm = Arena_new(128e3);
  // Testing string
  assert(Str_equals(S("Bonjour"), S("Bonjour")));
  Str s = S("Hello");
  assert(Str_equals(s, Str_copy(&perm, s.data, s.len)));
  // Testing JSON
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 5);
  JSON json_int = JSON_parse(&perm, "123");
  assert(*json_int.integer == 123);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 5);
  JSON json_int_array = JSON_parse(&perm, "[1,2,3]");
  assert(json_int_array.integer == NULL);
  assert(json_int_array.array->len == 3);
  int sum = 0;
  for (int i = 0; i < json_int_array.array->len; i++) {
    sum += *Vec_get(json_int_array.array, int, i);
  }
  assert(sum == 6);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 48);
  assert(*Vec_get(JSON_parse(&perm, "[1, 2, 3]").array, int, 1) == 2);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 88);
  JSON js = JSON_parse(&perm, "\"abc\"");
  assert(Str_equals(*js.string, S("abc")) == 1);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 91);
  return 0;
}
