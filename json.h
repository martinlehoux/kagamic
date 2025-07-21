#ifndef JSON_H
#define JSON_H
#include "memory.h"
#include "string.h"
#include "vec.h"

typedef struct {
    Vec keys; // Vec<Str>
    Vec values; // Vec<JSON>
    ptrdiff_t len;
} JSONObject;
typedef struct {
  int *integer;
  Vec *array; // Vec<JSON>
  Str *string;
  JSONObject *object;
} JSON;
JSON* JSONObject_get(JSONObject *obj, Str key);
JSON JSON_parse(Arena *a, char *src);
JSON JSON_Int(Arena *a, int val);
#endif
