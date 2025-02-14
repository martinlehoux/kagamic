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
  // Testing string
  Arena perm = Arena_new(128e3);
  assert(Str_equals(S("Bonjour"), S("Bonjour")));
  Str s = S("Hello");
  assert(Str_equals(s, Str_copy(&perm, s.data, s.len)));
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 5);
  // Testing JSON
  perm = Arena_new(128e3);
  JSON json_int = JSON_parse(&perm, "123");
  assert(*json_int.integer == 123);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 28);
  // JSON Int Array
  perm = Arena_new(128e3);
  JSON json_int_array = JSON_parse(&perm, "[1,2,3]");
  assert(json_int_array.integer == NULL);
  assert(json_int_array.array->len == 3);
  int ints[3];
  int sum = 0;
  for (int i = 0; i < json_int_array.array->len; i++) {
    JSON *item = Vec_get(json_int_array.array, JSON, i);
    ints[i] = *item->integer;
    sum += *item->integer;
  }
  printf("sum=%d\n", sum);
  assert(ints[0] == 1);
  assert(ints[1] == 2);
  assert(ints[2] == 3);
  assert(sum == 6);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 264);
  // JSON String
  perm = Arena_new(128e3);
  JSON js = JSON_parse(&perm, "\"abc\"");
  assert(Str_equals(*js.string, S("abc")) == 1);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 27);
  // JSON Nested hybrid array
  perm = Arena_new(128e3);
  JSON jaa = JSON_parse(&perm, "[[1,\"abc\"]]");
  JSON sub_array = *Vec_get(jaa.array, JSON, 0);
  JSON *val_1 = Vec_get(sub_array.array, JSON, 0);
  assert(*val_1->integer == 1);
  JSON *val_2 = Vec_get(sub_array.array, JSON, 1);
  assert(Str_equals(*val_2->string, S("abc")));
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(Arena_get_used(perm) == 203);
  return 0;
}
