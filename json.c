#include "json.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "json.h"
#include "string.h"
#include "types.h"

JSONObject *JSONObject_new(Arena *a) {
    JSONObject *object = new(a, JSONObject, 1);
    object->len = 0;
    object->keys = *Vec_new(a, Str, 2);
    object->values = *Vec_new(a, JSON, 2);
    return object;
}

i32 JSON_fprint(FILE *w, JSON json) {
    if (json.string != 0)
        return Str_fprint(w, *json.string);
    if (json.integer != 0)
        return fprintf(w, "%d", *json.integer);
    if (json.object != 0) {
        fprintf(w, "{");
        for (size i = 0; i < json.object->len; i++) {
            Str key = *Vec_get(&json.object->keys, Str, i);
            Str_fprint(w, key);
            fprintf(w, ": ");
            JSON value = *Vec_get(&json.object->values, JSON, i);
            JSON_fprint(w, value);
            if (i < json.object->len - 1)
                fprintf(w, ", ");
        }
        fprintf(w, "}");
        return 0;
    }
    if (json.array != 0) {
        fprintf(w, "[");
        for (size i = 0; i < json.array->len; i++) {
            JSON value = *Vec_get(json.array, JSON, i);
            JSON_fprint(w, value);
            if (i < json.array->len - 1)
                fprintf(w, ", ");
        }
        fprintf(w, "]");
        return 0;
    }
    return fprintf(w, "null");
}

typedef struct {
    JSON *value;
    uptr pos;
} parse_any_result;
parse_any_result parse_any(Arena *a, byte *src, uptr pos);

typedef struct {
    i32 *integer;
    f32 *floating;
    uptr pos;
} parse_number_result;

parse_number_result parse_number(Arena *a, byte *src, uptr start_pos) {
    parse_number_result result = {.pos = start_pos};
    i32 integer = 0;
    while (isdigit(src[result.pos])) {
        integer = integer * 10 + (src[result.pos] - '0');
        result.pos++;
    }
    if (src[result.pos] != '.') {
        result.integer = new(a, int, 1);
        *result.integer = integer;
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
    result.floating = new(a, f32, 1);
    *result.floating = (f32)integer + fractional_part;
    return result;
}

uptr absorb_whitespaces(byte *src, uptr start_pos) {
    uptr pos = start_pos;
    while (isspace(src[pos])) {
        pos++;
    }
    return pos;
}

typedef struct {
    Vec *items; // Vec<JSON>
    uptr pos;
} parse_array_result;

parse_array_result parse_array(Arena *a, byte *src, uptr start_pos) {
    assert(src[start_pos] == '[');
    parse_array_result result = {0};
    result.pos = start_pos + 1;
    result.items = Vec_new(a, JSON, 2);
    if (src[result.pos] == ']') {
        result.pos++;
        return result;
    }
    for (;;) {
        result.pos = absorb_whitespaces(src, result.pos);
        parse_any_result any_result = parse_any(a, src, result.pos);
        Vec_push(a, result.items, any_result.value);
        result.pos = absorb_whitespaces(src, any_result.pos);
        if (src[result.pos] == ',') {
            result.pos++;
        } else {
            break;
        }
    };
    assert(src[result.pos] == ']');
    result.pos++;
    return result;
}

typedef struct {
    Str *string;
    uptr pos;
} parse_string_result;

parse_string_result parse_string(Arena *a, byte *src, uptr start_pos) {
    assert(src[start_pos] == '"');
    parse_string_result result = {.pos = start_pos + 1};

    while (src[result.pos] != '"' && src[result.pos - 1] != '\\') {
        result.pos++;
    }
    assert(src[result.pos] == '"');
    result.string =
        Str_copy(a, &src[start_pos + 1], result.pos - start_pos - 1);
    result.pos++;

    return result;
}

typedef struct {
    JSONObject *object;
    uptr pos;
} parse_object_result;

parse_object_result parse_object(Arena *a, byte *src, uptr start_pos) {
    assert(src[start_pos] == '{');
    parse_object_result result = {.pos = start_pos + 1};
    result.object = JSONObject_new(a);
    start_pos = absorb_whitespaces(src, start_pos + 1);
    if (src[start_pos] == '}') {
        result.pos = start_pos + 1;
        return result;
    }
    for (;;) {
        assert(src[start_pos] == '"');
        parse_string_result str_result = parse_string(a, src, start_pos);
        Str *key = str_result.string;
        start_pos = absorb_whitespaces(src, str_result.pos);
        assert(src[start_pos] == ':');
        start_pos = absorb_whitespaces(src, start_pos + 1);
        parse_any_result any_result = parse_any(a, src, start_pos);
        result.object->len++;
        Vec_push(a, &result.object->keys, key);
        Vec_push(a, &result.object->values, any_result.value);
        start_pos = absorb_whitespaces(src, any_result.pos);
        if (src[start_pos] != ',') {
            break;
        }
        start_pos = absorb_whitespaces(src, start_pos + 1);
    }
    assert(src[start_pos] == '}');
    result.pos = start_pos + 1;
    return result;
}

parse_any_result parse_any(Arena *a, byte *src, uptr start_pos) {
    parse_any_result result = {.pos = start_pos};
    result.value = new(a, JSON, 1);
    if (isdigit(src[result.pos])) {
        parse_number_result number_result = parse_number(a, src, result.pos);
        if (number_result.integer != 0)
            result.value->integer = number_result.integer;
        else if (number_result.floating != 0)
            result.value->floating = number_result.floating;
        result.pos = number_result.pos;
    } else if (src[result.pos] == '[') {
        parse_array_result array_result = parse_array(a, src, result.pos);
        result.value->array = array_result.items;
        result.pos = array_result.pos;
    } else if (src[result.pos] == '"') {
        parse_string_result string_result = parse_string(a, src, result.pos);
        result.value->string = string_result.string;
        result.pos = string_result.pos;
    } else if (src[result.pos] == '{') {
        parse_object_result object_result = parse_object(a, src, result.pos);
        result.value->object = object_result.object;
        result.pos = object_result.pos;
    } else if (memcmp(src + result.pos, "true", 4) == 0) {
        result.value->boolean = new(a, i32, 1);
        *result.value->boolean = 1;
        result.pos = start_pos + 4;
    } else if (memcmp(src + result.pos, "false", 5) == 0) {
        result.value->boolean = new(a, i32, 1);
        *result.value->boolean = 0;
        result.pos = start_pos + 5;
    }

    return result;
}

JSON JSON_parse(Arena *a, byte *src) {
    parse_any_result result = parse_any(a, src, 0);
    return *result.value;
}

JSON JSON_Int(Arena *a, i32 val) {
    JSON json = {0};
    json.integer = new(a, i32, 1);
    *json.integer = val;
    return json;
}

JSON *JSONObject_get(JSONObject *obj, Str key) {
    for (size i = 0; i < obj->len; i++) {
        if (Str_equals(*Vec_get(&obj->keys, Str, i), key)) {
            return Vec_get(&obj->values, JSON, i);
        }
    }
    return 0;
}
