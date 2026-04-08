#include "../std/memory.h"
#include "../std/testing.h"
#include "../std/types.h"
#include "json.h"

#define BENCH_FILE "descriptive-1MB.json"

static MemoryFile json_parse_input;

static void setup_json_parse(Arena *a) {
    json_parse_input = load_file(a, BENCH_FILE);
}

i32 bench_json_parse(Arena *a) {
    JSON_parse(a, json_parse_input.content, json_parse_input.size);
    return 0;
}
