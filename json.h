#ifndef JSON_H
#define JSON_H
#include "memory.h"
#include "string.h"
#include "vec.h"

typedef struct {
  int *integer;
  Vec *array;
  Str *string;
} JSON;
JSON JSON_parse(Arena *a, char *src);
JSON JSON_Int(Arena *a, int val);
#endif
