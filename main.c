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
  assert(Str_equals(S("Bonjour"), S("Bonjour")));
  // Testing JSON
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  JSON json_int = JSON_parse(&perm, S("123"));
  assert(*json_int.integer == 123);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  JSON json_int_array = JSON_parse(&perm, S("[1,2,3]"));
  assert(json_int_array.integer == NULL);
  assert(json_int_array.array->len == 3);
  int sum = 0;
  for (int i = 0; i < json_int_array.array->len; i++) {
    sum += *Vec_get(json_int_array.array, int, i);
  }
  assert(sum == 6);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  assert(*Vec_get(JSON_parse(&perm, S("[1, 2, 3]")).array, int, 1) == 2);
  printf("Arena: mem=%d B\n", Arena_get_used(perm));
  Str a = S("\"abc\"");
  JSON s = JSON_parse(&perm, S("\"abc\""));
  // printf("%s (%d)\n", s.string->data, s.string->len);
  // assert(Str_equals(*JSON_parse(&perm, S("\"abc\"")).string, S("abc")) == 1);
  return 0;
}
