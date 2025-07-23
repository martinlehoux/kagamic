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
typedef enum {
    JSON_NULL,
    JSON_BOOL,
    JSON_INTEGER,
    JSON_FLOAT,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JSONType;
typedef struct {
    JSONType type;
    union {
        bool boolean;
        i32 integer;
        f32 floating;
        Vec array; // Vec<JSON>
        Str string;
        JSONObject object;
    } as;
} JSON;
JSON *JSONObject_get(JSONObject obj, Str key);
i32 JSON_fprint(FILE *w, JSON json);
JSON JSON_parse(Arena *a, byte *src);
JSON JSON_Int(Arena *a, i32 val);
#endif
