#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "include/assert.h"
#include "include/json.h"
#include "include/memory.h"
#include "include/types.h"
#include "include/vec.h"

// Load entire file into memory
byte *load_file(const char *filename, size *file_size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer and read file
    byte *buffer = malloc(*file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Could not allocate memory for file\n");
        fclose(file);
        return NULL;
    }

    size bytes_read = fread(buffer, 1, *file_size, file);
    if (bytes_read != *file_size) {
        fprintf(stderr, "Error: Could not read entire file\n");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[*file_size] = '\0'; // Null terminate
    fclose(file);
    return buffer;
}

// Get high resolution time in nanoseconds
double get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000.0 + ts.tv_nsec;
}

void run_benchmark(const char *filename, i32 iterations) {
    printf("Loading file: %s\n", filename);

    size file_size;
    byte *content = load_file(filename, &file_size);
    if (!content) return;

    printf("File size: %zu bytes\n", file_size);
    printf("Running %d iterations...\n\n", iterations);

    // Statistics tracking
    f32 timings[iterations];
    f32 timing_avg = 0.0;

    Arena arena = Arena_new(8e9);

    printf("Benchmarking...\n");
    for (i32 i = 0; i < iterations; i++) {
        Arena_reset(&arena);
        size used = Arena_get_used(arena);
        double start_time = get_time_ns();
        JSON_parse(&arena, content, file_size);
        double end_time = get_time_ns();

        timings[i] = end_time - start_time;
        timing_avg += timings[i];

        size new_used = Arena_get_used(arena);
        size allocated = new_used - used;
        printf("Iteration %d: %3.f ms\t%td MB\n", i + 1, timings[i] / 1000000,
               allocated / 1024 / 1024);

        if ((i + 1) % 100 == 0) {
            printf("Completed %d/%d iterations\n", i + 1, iterations);
        }
    }

    // Calculate statistics
    timing_avg /= iterations;
    f32 timing_var = 0.0;
    for (i32 i = 0; i < iterations; i++) {
        timing_var += (timings[i] - timing_avg) * (timings[i] - timing_avg);
    }
    timing_var /= iterations;
    f32 timing_stddev = sqrt(timing_var);
    double avg_throughput_mb_s =
        (file_size / (1024.0 * 1024.0)) / (timing_avg / 1000000000.0);

    printf("\n=== Benchmark Results ===\n");
    printf("%3.f ms ± %3.f ms per loop (mean ± std. dev. of %d runs, 1 loop "
           "each)\n",
           timing_avg / 1000000, timing_stddev / 1000000, iterations);
    printf("Throughput: %.2f MB/s\n", avg_throughput_mb_s);
}

i32 main(i32 argc, char *argv[]) {
    const char *filename = "";

    // Parse command line arguments
    assert(argc == 3, "main <file.json> <iterations>");
    filename = argv[1];
    i32 iterations = atoi(argv[2]);
    if (iterations <= 0) {
        fprintf(stderr, "Error: Invalid number of iterations\n");
        return 1;
    }

    printf("JSON Parser Benchmark\n");
    printf("====================\n");

    run_benchmark(filename, iterations);

    return 0;
}
