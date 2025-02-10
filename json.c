#include "json.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "string.h"

typedef struct {
  int value;
  uintptr_t pos;
} parse_integer_result;

parse_integer_result parse_integer(Arena *a, Str src, uintptr_t pos) {
  int *ints = new (a, int, 16); // TODO: reuse in parsing
  int i = 0;
  while (isdigit(src.data[pos + i])) {
    int val = (src.data[pos + i] - '0');
    ints[i] = val;
    i++;
  }
  int result = 0;
  for (int j = 0; j < i; j++) {
    result += ints[j] * pow(10, (i - j - 1));
  }
  printf("json.parse_integer: parsed int=%d at pos=%lu\n", result, pos);
  return (parse_integer_result){result, pos + i};
}

uintptr_t absorb_whitespaces(Str src, uintptr_t pos) {
  int i = 0;
  while (isspace(src.data[pos + i])) {
    i++;
  }
  return pos + i;
}

typedef struct {
  Vec *items;
  uintptr_t pos;
} parse_array_result;

parse_array_result parse_array(Arena *a, Str src, uintptr_t start_pos) {
  printf("json.parse_array: parsing array %s\n", src.data + start_pos);
  assert(src.data[start_pos] == '[');
  uintptr_t pos = start_pos + 1;
  Vec *items = Vec_new(a, JSON, 0);
  for (;;) {
    pos = absorb_whitespaces(src, pos);
    parse_integer_result result = parse_integer(a, src, pos);
    assert(result.pos > pos);
    items = Vec_push(a, items, &result.value);
    pos = result.pos;
    pos = absorb_whitespaces(src, pos);
    if (src.data[pos] == ',') {
      pos++;
    } else {
      break;
    }
  };
  assert(src.data[pos] == ']');

  return (parse_array_result){items, pos + 1};
}

typedef struct {
  Str string;
  uintptr_t pos;
} parse_string_result;

parse_string_result parse_string(Arena *a, Str src, uintptr_t pos) {
  printf("json.parse_string: %s\n", src.data + pos);
  assert(src.data[pos] == '"');
  int i = 1;

  while (src.data[pos + i] != '"' && src.data[pos + i - 1] != '\\') {
    i++;
    assert(i + pos <= src.len);
  }
  assert(src.data[pos + i]);
  Str s = Str_copy(a, &src.data[pos + 1], i - 1);

  return (parse_string_result){s, pos + i};
}

JSON JSON_parse(Arena *a, Str src) {
  JSON json = {0};
  int pos = 0;
  printf("json.parse: src=%s\n", src.data);
  if (isdigit(src.data[pos])) {
    parse_integer_result result = parse_integer(a, src, 0);
    json.integer = &result.value;
    pos = result.pos;
  }
  if (src.data[pos] == '[') {
    parse_array_result result = parse_array(a, src, pos);
    json.array = result.items;
    pos = result.pos;
  }
  if (src.data[pos] == '"') {
    parse_string_result result = parse_string(a, src, pos);
    json.string = &result.string;
    pos = result.pos;
  }

  return json;
}
