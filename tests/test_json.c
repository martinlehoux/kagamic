#include "../include/json.h"
#include "../include/testing.h"

i32 test_json_integer() {
    Arena perm = Arena_new(128e3);

    JSON json_int = JSON_parse(&perm, "123");

    t_assert(json_int.as.integer == 123);
    t_assert_equal(Arena_get_used(perm), 0);
    return 0;
}

i32 test_json_int_array() {
    Arena perm = Arena_new(128e3);

    JSON json_int_array = JSON_parse(&perm, "[1,2,3]");

    t_assert(json_int_array.type == JSON_ARRAY);
    t_assert_equal(json_int_array.as.array.len, 3);
    int ints[3];
    int sum = 0;
    for (int i = 0; i < json_int_array.as.array.len; i++) {
        JSON *item = Vec_get(json_int_array.as.array, JSON, i);
        ints[i] = item->as.integer;
        sum += item->as.integer;
    }
    t_assert(ints[0] == 1);
    t_assert(ints[1] == 2);
    t_assert(ints[2] == 3);
    t_assert(sum == 6);
    t_assert_equal(Arena_get_used(perm), 576);

    return 0;
}

i32 test_json_empty_array() {
    Arena perm = Arena_new(128e3);

    JSON json_empty_array = JSON_parse(&perm, "[]");

    t_assert(json_empty_array.as.array.len == 0);
    t_assert_equal(Arena_get_used(perm), 0); // TODO: weird

    return 0;
}

i32 test_json_nested_array() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "[[1,\"abc\"]]");

    JSON sub_array = *Vec_get(doc.as.array, JSON, 0);
    JSON val_1 = *Vec_get(sub_array.as.array, JSON, 0);
    t_assert(val_1.as.integer == 1);
    JSON val_2 = *Vec_get(sub_array.as.array, JSON, 1);
    Str str = val_2.as.string;
    Str expected = S("abc");
    t_assert(Str_equals(str, expected));
    t_assert_equal(Arena_get_used(perm), 387);

    return 0;
}

i32 test_json_string() {
    Arena perm = Arena_new(128e3);

    JSON js = JSON_parse(&perm, "\"abc\"");

    t_assert(Str_equals(js.as.string, S("abc")));
    t_assert_equal(Arena_get_used(perm), 3);

    return 0;
}

i32 test_json_empty_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{}");

    t_assert_equal(doc.as.object.len, 0);
    t_assert_equal(Arena_get_used(perm), 224);

    return 0;
}

i32 test_json_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{\"key\": \"value\"}");

    t_assert_equal(doc.as.object.len, 1);
    t_assert(Str_equals(*Vec_get(doc.as.object.keys, Str, 0), S("key")));
    JSON value = *JSONObject_get(doc.as.object, S("key"));
    t_assert(Str_equals(value.as.string, S("value")));
    t_assert_equal(Arena_get_used(perm), 232);

    return 0;
}

i32 test_json_boolean() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{\"a\": true, \"b\": false}");

    JSON a = *JSONObject_get(doc.as.object, S("a"));
    t_assert(a.as.boolean == true);
    JSON b = *JSONObject_get(doc.as.object, S("b"));
    t_assert(b.as.boolean == false);
    t_assert_equal(Arena_get_used(perm), 226);

    return 0;
}

i32 test_json_float() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "[1.2, 2.3]");
    t_assert_equal(doc.as.array.len, 2);
    JSON first = *Vec_get(doc.as.array, JSON, 0);
    t_assert_fequal(first.as.floating, 1.2);
    JSON second = *Vec_get(doc.as.array, JSON, 1);
    t_assert_fequal(second.as.floating, 2.3);
    t_assert_equal(Arena_get_used(perm), 192);

    return 0;
}

i32 test_json_null() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "[null]");
    t_assert_equal(doc.as.array.len, 1);
    JSON first = *Vec_get(doc.as.array, JSON, 0);
    t_assert(first.type == JSON_NULL);
    t_assert_equal(Arena_get_used(perm), 192);

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
    JSON target_size_mb = *JSONObject_get(doc.as.object, S("target_size_mb"));
    t_assert(target_size_mb.as.integer == 1);
    JSON generated_at = *JSONObject_get(doc.as.object, S("generated_at"));
    t_assert(
        Str_equals(generated_at.as.string, S("2025-07-22T10:36:30.974926")));

    return 0;
}

i32 test_json_nested_object() {
    Arena perm = Arena_new(128e3);

    JSON doc = JSON_parse(&perm, "{\"1\":{\"2\":{}}}");

    JSON lvl_1 = *JSONObject_get(doc.as.object, S("1"));
    t_assert(lvl_1.type == JSON_OBJECT);
    JSON lvl_2 = *JSONObject_get(lvl_1.as.object, S("2"));
    t_assert(lvl_2.type == JSON_OBJECT);
    return 0;
}

i32 test_json_complex_multiline() {
    Arena perm = Arena_new(128e3);
    JSON doc =
        JSON_parse(&perm, "{\n"
                          "    \"id\": 819228,\n"
                          "    \"username\": \"rCrRv4PRnUNuAy\",\n"
                          "    \"email\": \"xzbwrzj@zwod.com\",\n"
                          "    \"first_name\": \"4WH7g7PsOEm5\",\n"
                          "    \"last_name\": \"6ulndxUgjbIayl\",\n"
                          "    \"age\": 30,\n"
                          "    \"phone\": \"+1-529-712-3931\",\n"
                          "    \"is_active\": false,\n"
                          "    \"balance\": 9347.49,\n"
                          "    \"tags\": [\n"
                          "      \"TWvZi0\",\n"
                          "      \"pM0TPv9X\"\n"
                          "    ],\n"
                          "    \"metadata\": {\n"
                          "      \"last_login\": \"2024-06-13T21:22:32Z\",\n"
                          "      \"login_count\": 245,\n"
                          "      \"preferences\": {\n"
                          "        \"theme\": \"auto\",\n"
                          "        \"notifications\": true,\n"
                          "        \"language\": \"en\"\n"
                          "      }\n"
                          "    }\n"
                          "  }");

    JSON id = *JSONObject_get(doc.as.object, S("id"));
    t_assert(id.as.integer == 819228);
    JSON metadata = *JSONObject_get(doc.as.object, S("metadata"));
    JSON preferences = *JSONObject_get(metadata.as.object, S("preferences"));
    JSON theme = *JSONObject_get(preferences.as.object, S("theme"));
    t_assert(Str_equals(theme.as.string, S("auto")));

    return 0;
}
