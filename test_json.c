#include "json.h"
#include "testing.h"

char test_json_integer() {
  Arena perm = Arena_new(128e3);

  JSON json_int = JSON_parse(&perm, "123");

  t_assert(*json_int.integer == 123);
  t_assert(Arena_get_used(perm) == 28);
  return 0;
}

char test_json_int_array() {
  Arena perm = Arena_new(128e3);

  JSON json_int_array = JSON_parse(&perm, "[1,2,3]");

  t_assert(json_int_array.integer == NULL);
  t_assert(json_int_array.array->len == 3);
  int ints[3];
  int sum = 0;
  for (int i = 0; i < json_int_array.array->len; i++) {
    JSON *item = Vec_get(json_int_array.array, JSON, i);
    ints[i] = *item->integer;
    sum += *item->integer;
  }
  t_assert(ints[0] == 1);
  t_assert(ints[1] == 2);
  t_assert(ints[2] == 3);
  t_assert(sum == 6);
  t_assert(Arena_get_used(perm) == 264);

  return 0;
}

char test_json_nested_array() {
  Arena perm = Arena_new(128e3);

  JSON doc = JSON_parse(&perm, "[[1,\"abc\"]]");

  JSON sub_array = *Vec_get(doc.array, JSON, 0);
  JSON val_1 = *Vec_get(sub_array.array, JSON, 0);
  t_assert(*val_1.integer == 1);
  JSON val_2 = *Vec_get(sub_array.array, JSON, 1);
  Str str = *val_2.string;
  Str expected = S("abc");
  t_assert(Str_equals(str, expected));
  t_assert(Arena_get_used(perm) == 203);

  return 0;
}

char test_json_string() {
  Arena perm = Arena_new(128e3);

  JSON js = JSON_parse(&perm, "\"abc\"");

  t_assert(Str_equals(*js.string, S("abc")));
  t_assert(Arena_get_used(perm) == 27);

  return 0;
}
