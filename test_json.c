#include "json.h"
#include "testing.h"

i32 test_json_integer() {
    Arena perm = Arena_new(128e3);

    JSON json_int = JSON_parse(&perm, "123");

    t_assert(*json_int.integer == 123);
    t_assert_equal(Arena_get_used(perm), 60);
    return 0;
}

i32 test_json_int_array() {
    Arena perm = Arena_new(128e3);

    JSON json_int_array = JSON_parse(&perm, "[1,2,3]");

    t_assert(json_int_array.integer == 0);
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
    t_assert_equal(Arena_get_used(perm), 624);

    return 0;
}

i32 test_json_empty_array() {
    Arena perm = Arena_new(128e3);

    JSON json_empty_array = JSON_parse(&perm, "[]");

    t_assert(json_empty_array.array->len == 0);
    t_assert_equal(Arena_get_used(perm), 208);

    return 0;
}

i32 test_json_nested_array() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "[[1,\"abc\"]]");

    JSON sub_array = *Vec_get(doc.array, JSON, 0);
    JSON val_1 = *Vec_get(sub_array.array, JSON, 0);
    t_assert(*val_1.integer == 1);
    JSON val_2 = *Vec_get(sub_array.array, JSON, 1);
    Str str = *val_2.string;
    Str expected = S("abc");
    t_assert(Str_equals(str, expected));
    t_assert_equal(Arena_get_used(perm), 555);

    return 0;
}

i32 test_json_string() {
    Arena perm = Arena_new(128e3);

    JSON js = JSON_parse(&perm, "\"abc\"");

    t_assert(Str_equals(*js.string, S("abc")));
    t_assert_equal(Arena_get_used(perm), 75);

    return 0;
}

i32 test_json_empty_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{}");

    t_assert_equal(doc.object->len, 0);
    t_assert_equal(Arena_get_used(perm), 368);

    return 0;
}

i32 test_json_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{\"key\": \"value\"}");

    t_assert_equal(doc.object->len, 1);
    t_assert(Str_equals(*Vec_get(&doc.object->keys, Str, 0), S("key")));
    JSON value = *JSONObject_get(doc.object, S("key"));
    t_assert(Str_equals(*value.string, S("value")));
    t_assert_equal(Arena_get_used(perm), 469);

    return 0;
}

i32 test_json_boolean() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{\"a\": true, \"b\": false}");

    JSON a = *JSONObject_get(doc.object, S("a"));
    t_assert(*a.boolean == 1);
    JSON b = *JSONObject_get(doc.object, S("b"));
    t_assert(*b.boolean == 0);
    t_assert_equal(Arena_get_used(perm), 540);

    return 0;
}

i32 test_json_float() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "[1.2, 2.3]");
    t_assert_equal(doc.array->len, 2);
    JSON first = *Vec_get(doc.array, JSON, 0);
    t_assert_fequal(*first.floating, 1.2);
    JSON second = *Vec_get(doc.array, JSON, 1);
    t_assert_fequal(*second.floating, 2.3);
    t_assert_equal(Arena_get_used(perm), 332);

    return 0;
}

i32 test_json_null() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "[null]");
    t_assert_equal(doc.array->len, 1);
    JSON first = *Vec_get(doc.array, JSON, 0);
    t_assert(first.null != 0);
    t_assert_equal(Arena_get_used(perm), 268);

    return 0;
}

i32 test_json_multiline() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(
        &perm, "{\n"
               "    \"generated_at\": \"2025-07-22T10:36:30.974926\",\n"
               "    \"target_size_mb\": 1,\n"
               "    \"generator_version\": \"1.0\"\n"
               "}");
    JSON target_size_mb = *JSONObject_get(doc.object, S("target_size_mb"));
    t_assert(*target_size_mb.integer == 1);
    JSON generated_at = *JSONObject_get(doc.object, S("generated_at"));
    t_assert(Str_equals(*generated_at.string, S("2025-07-22T10:36:30.974926")));

    return 0;
}

i32 test_json_nested_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{\"1\":{\"2\":{}}}");

    JSON lvl_1 = *JSONObject_get(doc.object, S("1"));
    t_assert(lvl_1.object != 0);
    JSON lvl_2 = *JSONObject_get(lvl_1.object, S("2"));
    t_assert(lvl_2.object != 0);
    return 0;
}
