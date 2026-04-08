#include <stdio.h>

#include "assert.h"
#include "memory.h"
#include "str.h"
#include "testing.h"
#include "types.h"

MemoryFile load_file(Arena *a, const char *filename) {
    FILE *file = fopen(filename, "rb");
    assert(file != 0, "failed to open file: %s", filename);
    MemoryFile mem_file = {0};

    // Get file size
    fseek(file, 0, SEEK_END);
    mem_file.size = ftell(file);
    fseek(file, 0, SEEK_SET);

    mem_file.content = new(a, byte, mem_file.size + 1);

    size bytes_read = fread(mem_file.content, 1, mem_file.size, file);
    assert(bytes_read == mem_file.size, "failed to read entire file: %s",
           filename);

    mem_file.content[mem_file.size] = '\0'; // Null terminate
    fclose(file);
    return mem_file;
}
