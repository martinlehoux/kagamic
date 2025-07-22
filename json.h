#ifndef JSON_H
#define JSON_H
#include "memory.h"
#include "string.h"
#include "vec.h"

typedef struct {
    Vec keys;   // Vec<Str>
    Vec values; // Vec<JSON>
    size len;
} JSONObject;
typedef struct {
    i32 *boolean;
    i32 *integer;
    Vec *array; // Vec<JSON>
    Str *string;
    JSONObject *object;
} JSON;
JSON *JSONObject_get(JSONObject *obj, Str key);
i32 JSON_fprint(FILE *w, JSON json);
JSON JSON_parse(Arena *a, byte *src);
JSON JSON_Int(Arena *a, i32 val);
#endif
