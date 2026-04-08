#include <ctype.h>
#include <immintrin.h>
#include <math.h>
#include <stdbit.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../std/assert.h"
#include "../std/str.h"
#include "../std/types.h"
#include "json.h"

JSONObject *JSONObject_new(Arena *a) {
    JSONObject *object = new(a, JSONObject, 1);
    object->len = 0;
    object->keys = Vec_new(a, StrView, 2);
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

JSON JSON_string(StrView value) {
    JSON json = {0};
    json.type = JSON_STRING;
    json.as.string = value;
    return json;
}

JSON JSON_object(size len, Vec keys, Vec values) {
    assert(len == keys.len && len == values.len, "length mismatch");
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
            StrView key = *Vec_get(json.as.object.keys, StrView, i);
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
    assert(false, "unknown json type %d", json.type);
}

typedef struct {
    JSON value;
    uptr pos;
} parse_result;

#define assert_parse(condition, msg, src, pos)                                 \
    assert(condition, "%s (%c at %lu)", msg, src[pos], pos)

parse_result parse_any(Arena *a, byte *src, uptr pos, uptr max_pos);

parse_result parse_number(byte *src, uptr start_pos) {
    parse_result result = {0};
    byte *end;
    f32 number = strtod(src + start_pos, &end);
    assert_parse(end != src, "invalide number", src, result.pos);
    result.pos += end - src;
    result.value =
        floor(number) == number ? JSON_integer(number) : JSON_float(number);
    return result;
}

uptr absorb_whitespaces(byte *src, uptr start_pos, uptr max_pos) {
    uptr pos = start_pos;
    while (pos < max_pos && isspace(src[pos])) {
        pos++;
    }
    return pos;
}

parse_result parse_array(Arena *a, byte *src, uptr start_pos, uptr max_pos) {
    assert_parse(src[start_pos] == '[', "invalid array start", src, start_pos);
    parse_result result = {0};
    result.pos = start_pos + 1;
    if (src[result.pos] == ']') {
        result.pos++;
        return result;
    }
    Vec items = Vec_new(a, JSON, 6);
    for (;;) {
        result.pos = absorb_whitespaces(src, result.pos, max_pos);
        parse_result item_result = parse_any(a, src, result.pos, max_pos);
        Vec_push(a, &items, &item_result.value);
        result.pos = absorb_whitespaces(src, item_result.pos, max_pos);
        if (src[result.pos] == ',') {
            result.pos++;
        } else {
            break;
        }
    };
    assert_parse(src[result.pos] == ']', "invalid array end", src, result.pos);
    result.pos++;
    result.value = JSON_array(items);
    return result;
}

parse_result parse_string(byte *src, uptr start_pos, uptr max_pos) {
    assert_parse(src[start_pos] == '"', "invalid string start", src, start_pos);
    parse_result result = {0};
    result.pos = start_pos + 1;

    // SIMD optimization: process 32 bytes at a time using AVX2
    bytex32 quote_bytes = _mm256_set1_epi8('"');

    for (; result.pos + 32 <= max_pos; result.pos += 32) {
        bytex32 chunk = _mm256_loadu_si256((bytex32 *)(src + result.pos));
        i32 quote_mask =
            _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, quote_bytes));

        if (quote_mask != 0) {
            result.pos += stdc_trailing_zeros_ui(quote_mask);
            uptr backslash_count = 0;
            while (backslash_count < result.pos &&
                   src[result.pos - backslash_count - 1] == '\\') {
                backslash_count++;
            }
            if (!(backslash_count & 1)) break;
            result.pos++;
        };
    }

    // Fall back to original byte-by-byte processing for the remaining bytes
    for (; result.pos < max_pos; result.pos++) {
        if (src[result.pos] == '"') {
            uptr backslash_count = 0;
            while (backslash_count < result.pos &&
                   src[result.pos - backslash_count - 1] == '\\') {
                backslash_count++;
            }
            if (!(backslash_count & 1)) break;
        }
    }

    assert_parse(src[result.pos] == '"', "invalid string end", src, result.pos);
    result.value =
        JSON_string(Str_view(&src[start_pos + 1], result.pos - start_pos - 1));
    result.pos++;

    return result;
}

parse_result parse_object(Arena *a, byte *src, uptr start_pos, uptr max_pos) {
    assert_parse(src[start_pos] == '{', "invalid object start", src, start_pos);
    parse_result result = {0};
    result.pos = absorb_whitespaces(src, start_pos + 1, max_pos);
    size len = 0;
    Vec keys = Vec_new(a, StrView, 6);
    Vec values = Vec_new(a, JSON, 6);
    if (src[result.pos] == '}') {
        result.pos++;
        result.value = JSON_object(len, keys, values);
        return result;
    }
    for (;;) {
        parse_result key_result = parse_string(src, result.pos, max_pos);
        result.pos = absorb_whitespaces(src, key_result.pos, max_pos);
        assert_parse(src[result.pos] == ':', "invalid keyval delim", src,
                     result.pos);
        result.pos = absorb_whitespaces(src, result.pos + 1, max_pos);
        parse_result value_result = parse_any(a, src, result.pos, max_pos);
        len++;
        assert(key_result.value.type == JSON_STRING,
               "invalid json type for key: %d", key_result.value.type);
        Vec_push(a, &keys, &key_result.value.as.string);
        Vec_push(a, &values, &value_result.value);
        result.pos = absorb_whitespaces(src, value_result.pos, max_pos);
        if (src[result.pos] != ',') {
            break;
        }
        result.pos = absorb_whitespaces(src, result.pos + 1, max_pos);
    }
    assert_parse(src[result.pos] == '}', "invalid object end", src, result.pos);
    result.pos++;
    result.value = JSON_object(len, keys, values);
    return result;
}

parse_result parse_any(Arena *a, byte *src, uptr pos, uptr max_pos) {
    pos = absorb_whitespaces(src, pos, max_pos);
    if (isdigit(src[pos]) || src[pos] == '-') return parse_number(src, pos);
    if (src[pos] == '[') return parse_array(a, src, pos, max_pos);
    if (src[pos] == '"') return parse_string(src, pos, max_pos);
    if (src[pos] == '{') return parse_object(a, src, pos, max_pos);
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
    assert_parse(false, "unexpected input", src, pos);
}

JSON JSON_parse(Arena *a, byte *src, uptr len) {
    return parse_any(a, src, 0, len).value;
}

JSON *JSONObject_get(JSONObject obj, Str key) {
    for (size i = 0; i < obj.len; i++) {
        StrView tmp_key = *Vec_get(obj.keys, StrView, i);
        if (Str_equals(tmp_key, key)) return Vec_get(obj.values, JSON, i);
    }
    return 0;
}
