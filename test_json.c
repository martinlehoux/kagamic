#include "json.h"
#include "testing.h"
#include <stdio.h>

char test_json_integer() {
  Arena perm = Arena_new(128e3);

  JSON json_int = JSON_parse(&perm, "123");

  t_assert(*json_int.integer == 123);
  t_assert_equal(Arena_get_used(perm), 36);
  return 0;
}

char test_json_int_array() {
  Arena perm = Arena_new(128e3);

  JSON json_int_array = JSON_parse(&perm, "[1,2,3]");

  t_assert(json_int_array.integer == NULL);
  t_assert_equal(json_int_array.array->len, 3);
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
  t_assert_equal(Arena_get_used(perm), 344);

  return 0;
}

char test_json_nested_array() {
  Arena perm = Arena_new(128e3);

  JSON doc = JSON_parse(&perm, "[[1,\"abc\"]]");

  t_assert_equal(Arena_get_used(perm), 283);
  JSON sub_array = *Vec_get(doc.array, JSON, 0);
  JSON val_1 = *Vec_get(sub_array.array, JSON, 0);
  t_assert(*val_1.integer == 1);
  JSON val_2 = *Vec_get(sub_array.array, JSON, 1);
  Str str = *val_2.string;
  Str expected = S("abc");
  t_assert(Str_equals(str, expected));
  t_assert_equal(Arena_get_used(perm), 283);

  return 0;
}

char test_json_string() {
  Arena perm = Arena_new(128e3);

  JSON js = JSON_parse(&perm, "\"abc\"");

  t_assert_equal(Arena_get_used(perm), 51);
  t_assert(Str_equals(*js.string, S("abc")));
  t_assert_equal(Arena_get_used(perm), 51);

  return 0;
}

char test_json_empty_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{}");

    t_assert_equal(doc.object->len, 0);
    t_assert_equal(Arena_get_used(perm), 216);

    return 0;
}

char test_json_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{\"key\": \"value\"}");

    t_assert_equal(Arena_get_used(perm), 293);
    t_assert_equal(doc.object->len, 1);
    t_assert(Str_equals(*Vec_get(&doc.object->keys, Str, 0), S("key")));
    t_assert(Str_equals(*JSONObject_get(doc.object, S("key"))->string, S("value")))
    t_assert_equal(Arena_get_used(perm), 293);

    return 0;
}
