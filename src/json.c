#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/json.h"
#include "../include/str.h"
#include "../include/types.h"

JSONObject *JSONObject_new(Arena *a) {
    JSONObject *object = new(a, JSONObject, 1);
    object->len = 0;
    object->keys = Vec_new(a, Str, 2);
    object->values = Vec_new(a, JSON, 2);
    return object;
}

JSON JSON_integer(i32 value) {
    JSON json = {0};
    json.type = JSON_INTEGER;
    json.as.integer = value;
    return json;
}

JSON JSON_float(f32 value) {
    JSON json = {0};
    json.type = JSON_FLOAT;
    json.as.floating = value;
    return json;
}

JSON JSON_array(Vec items) {
    JSON json = {0};
    json.type = JSON_ARRAY;
    json.as.array = items;
    return json;
}

JSON JSON_string(Str value) {
    JSON json = {0};
    json.type = JSON_STRING;
    json.as.string = value;
    return json;
}

JSON JSON_object(size len, Vec keys, Vec values) {
    assert(len == keys.len && len == values.len);
    JSON json = {0};
    json.type = JSON_OBJECT;
    json.as.object.len = len;
    json.as.object.keys = keys;
    json.as.object.values = values;
    return json;
}

JSON JSON_boolean(bool value) {
    JSON json = {0};
    json.type = JSON_BOOL;
    json.as.boolean = value;
    return json;
}

JSON JSON_null() {
    JSON json = {0};
    json.type = JSON_NULL;
    return json;
}

i32 JSON_fprint(FILE *w, JSON json) {
    switch (json.type) {
    case JSON_STRING:
        fprintf(w, "\"");
        Str_fprint(w, json.as.string);
        return fprintf(w, "\"");
    case JSON_INTEGER:
        return fprintf(w, "%d", json.as.integer);
    case JSON_FLOAT:
        return fprintf(w, "%f", json.as.floating);
    case JSON_NULL:
        return fprintf(w, "null");
    case JSON_BOOL:
        return fprintf(w, "%s", json.as.boolean ? "true" : "false");
    case JSON_ARRAY:
        fprintf(w, "[");
        for (size i = 0; i < json.as.array.len; i++) {
            JSON value = *Vec_get(json.as.array, JSON, i);
            JSON_fprint(w, value);
            if (i < json.as.array.len - 1) fprintf(w, ", ");
        }
        fprintf(w, "]");
        return 0;
    case JSON_OBJECT:
        fprintf(w, "{");
        for (size i = 0; i < json.as.object.len; i++) {
            Str key = *Vec_get(json.as.object.keys, Str, i);
            fprintf(w, "\"");
            Str_fprint(w, key);
            fprintf(w, "\": ");
            JSON value = *Vec_get(json.as.object.values, JSON, i);
            JSON_fprint(w, value);
            if (i < json.as.object.len - 1) fprintf(w, ", ");
        }
        fprintf(w, "}");
        return 0;
    }
    assert(false);
}

typedef struct {
    JSON value;
    uptr pos;
} parse_result;

parse_result parse_any(Arena *a, byte *src, uptr pos);

parse_result parse_number(byte *src, uptr start_pos) {
    parse_result result = {0};
    result.pos = start_pos;
    i32 integer = 0;
    while (isdigit(src[result.pos])) {
        integer = integer * 10 + (src[result.pos] - '0');
        result.pos++;
    }
    if (src[result.pos] != '.') {
        result.value = JSON_integer(integer);
        return result;
    }
    result.pos++;
    f32 fractional_part = 0.0;
    f32 decimal_place = 0.1;
    while (isdigit(src[result.pos])) {
        fractional_part += (src[result.pos] - '0') * decimal_place;
        decimal_place *= 0.1;
        result.pos++;
    }
    result.value = JSON_float((f32)integer + fractional_part);
    return result;
}

uptr absorb_whitespaces(byte *src, uptr start_pos) {
    uptr pos = start_pos;
    while (isspace(src[pos])) {
        pos++;
    }
    return pos;
}

parse_result parse_array(Arena *a, byte *src, uptr start_pos) {
    assert(src[start_pos] == '[');
    parse_result result = {0};
    result.pos = start_pos + 1;
    if (src[result.pos] == ']') {
        result.pos++;
        return result;
    }
    Vec items = Vec_new(a, JSON, 2);
    for (;;) {
        result.pos = absorb_whitespaces(src, result.pos);
        parse_result item_result = parse_any(a, src, result.pos);
        Vec_push(a, &items, &item_result.value);
        result.pos = absorb_whitespaces(src, item_result.pos);
        if (src[result.pos] == ',') {
            result.pos++;
        } else {
            break;
        }
    };
    assert(src[result.pos] == ']');
    result.pos++;
    result.value = JSON_array(items);
    return result;
}

parse_result parse_string(Arena *a, byte *src, uptr start_pos) {
    assert(src[start_pos] == '"');
    parse_result result = {0};
    result.pos = start_pos + 1;

    while (src[result.pos] != '"' && src[result.pos - 1] != '\\') {
        result.pos++;
    }
    assert(src[result.pos] == '"');
    result.value = JSON_string(
        Str_copy(a, &src[start_pos + 1], result.pos - start_pos - 1));
    result.pos++;

    return result;
}

parse_result parse_object(Arena *a, byte *src, uptr start_pos) {
    assert(src[start_pos] == '{');
    parse_result result = {0};
    result.pos = absorb_whitespaces(src, start_pos + 1);
    size len = 0;
    Vec keys = Vec_new(a, Str, 2);
    Vec values = Vec_new(a, JSON, 2);
    if (src[result.pos] == '}') {
        result.pos++;
        result.value = JSON_object(len, keys, values);
        return result;
    }
    for (;;) {
        assert(src[result.pos] == '"');
        parse_result key_result = parse_string(a, src, result.pos);
        result.pos = absorb_whitespaces(src, key_result.pos);
        assert(src[result.pos] == ':');
        result.pos = absorb_whitespaces(src, result.pos + 1);
        parse_result value_result = parse_any(a, src, result.pos);
        len++;
        assert(key_result.value.type == JSON_STRING);
        Vec_push(a, &keys, &key_result.value.as.string);
        Vec_push(a, &values, &value_result.value);
        result.pos = absorb_whitespaces(src, value_result.pos);
        if (src[result.pos] != ',') {
            break;
        }
        result.pos = absorb_whitespaces(src, result.pos + 1);
    }
    assert(src[result.pos] == '}');
    result.pos++;
    result.value = JSON_object(len, keys, values);
    return result;
}

parse_result parse_any(Arena *a, byte *src, uptr pos) {
    pos = absorb_whitespaces(src, pos);
    if (isdigit(src[pos])) return parse_number(src, pos);
    if (src[pos] == '[') return parse_array(a, src, pos);
    if (src[pos] == '"') return parse_string(a, src, pos);
    if (src[pos] == '{') return parse_object(a, src, pos);
    if (memcmp(src + pos, "true", 4) == 0) {
        parse_result result = {0};
        result.value = JSON_boolean(true);
        result.pos = pos + 4;
        return result;
    }
    if (memcmp(src + pos, "false", 5) == 0) {
        parse_result result = {0};
        result.value = JSON_boolean(false);
        result.pos = pos + 5;
        return result;
    }
    if (memcmp(src + pos, "null", 4) == 0) {
        parse_result result = {0};
        result.value = JSON_null();
        result.pos = pos + 4;
        return result;
    }
    assert(false);
}

JSON JSON_parse(Arena *a, byte *src) { return parse_any(a, src, 0).value; }

JSON *JSONObject_get(JSONObject obj, Str key) {
    for (size i = 0; i < obj.len; i++) {
        if (Str_equals(*Vec_get(obj.keys, Str, i), key)) {
            return Vec_get(obj.values, JSON, i);
        }
    }
    assert(false);
}
