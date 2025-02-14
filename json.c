#include "json.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "string.h"

typedef struct {
  JSON *value;
  uintptr_t pos;
} parse_any_result;
parse_any_result parse_any(Arena *a, char *src, uintptr_t pos);

typedef struct {
  int *value;
  uintptr_t pos;
} parse_integer_result;

parse_integer_result parse_integer(Arena *a, char *src, uintptr_t pos) {
  char ints[16];
  int i = 0;
  while (isdigit(src[pos + i])) {
    int val = (src[pos + i] - '0');
    ints[i] = val;
    i++;
  }
  int *result = new (a, int, 1);
  for (int j = 0; j < i; j++) {
    *result += ints[j] * pow(10, (i - j - 1));
  }
  printf("json.parse_integer: parsed int=%d at pos=%lu\n", *result, pos);
  return (parse_integer_result){result, pos + i};
}

uintptr_t absorb_whitespaces(char *src, uintptr_t pos) {
  int i = 0;
  while (isspace(src[pos + i])) {
    i++;
  }
  return pos + i;
}

typedef struct {
  Vec *items;
  uintptr_t pos;
} parse_array_result;

parse_array_result parse_array(Arena *a, char *src, uintptr_t pos) {
  printf("json.parse_array: parsing array %s\n", src + pos);
  assert(src[pos] == '[');
  pos++;
  Vec items = Vec_new(a, JSON, 2);
  for (;;) {
    pos = absorb_whitespaces(src, pos);
    parse_any_result result = parse_any(a, src, pos);
    Vec_push(a, &items, result.value);
    pos = result.pos;
    pos = absorb_whitespaces(src, pos);
    if (src[pos] == ',') {
      pos++;
    } else {
      break;
    }
  };
  assert(src[pos] == ']');

  return (parse_array_result){&items, pos + 1};
}

typedef struct {
  Str string;
  uintptr_t pos;
} parse_string_result;

parse_string_result parse_string(Arena *a, char *src, uintptr_t pos) {
  printf("json.parse_string: %s\n", src + pos);
  assert(src[pos] == '"');
  int i = 1;

  while (src[pos + i] != '"' && src[pos + i - 1] != '\\') {
    i++;
  }
  assert(src[pos + i] == '"');
  Str s = Str_copy(a, &src[pos + 1], i - 1);

  return (parse_string_result){s, pos + i + 1};
}

parse_any_result parse_any(Arena *a, char *src, uintptr_t pos) {
  JSON *value = new (a, JSON, 1);
  if (isdigit(src[pos])) {
    parse_integer_result result = parse_integer(a, src, pos);
    value->integer = result.value;
    pos = result.pos;
  } else if (src[pos] == '[') {
    parse_array_result result = parse_array(a, src, pos);
    value->array = result.items;
    pos = result.pos;
  } else if (src[pos] == '"') {
    parse_string_result result = parse_string(a, src, pos);
    value->string = &result.string;
    pos = result.pos;
  }

  return (parse_any_result){value, pos};
}

JSON JSON_parse(Arena *a, char *src) {
  parse_any_result result = parse_any(a, src, 0);
  return *result.value;
}

JSON JSON_Int(Arena *a, int val) {
  JSON json = {0};
  json.integer = new (a, int, 1);
  *json.integer = val;
  return json;
}
