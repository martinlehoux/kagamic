#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "json.h"
#include "memory.h"
#include "types.h"

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
    double total_time = 0.0;
    double min_time = 1e9;
    double max_time = 0.0;

    Arena arena = Arena_new(8e9);

    printf("Benchmarking...\n");
    for (i32 i = 0; i < iterations; i++) {
        Arena_reset(&arena);
        size used = Arena_get_used(arena);
        double start_time = get_time_ns();
        JSON result = JSON_parse(&arena, content);
        double end_time = get_time_ns();

        double elapsed = end_time - start_time;
        total_time += elapsed;

        if (elapsed < min_time) min_time = elapsed;
        if (elapsed > max_time) max_time = elapsed;

        size new_used = Arena_get_used(arena);
        size allocated = new_used - used;
        printf("Iteration %d: %td MB allocated\n", i + 1,
               allocated / 1024 / 1024);

        if ((i + 1) % 100 == 0) {
            printf("Completed %d/%d iterations\n", i + 1, iterations);
        }
    }

    // Calculate statistics
    double avg_time = total_time / iterations;
    double avg_time_ms = avg_time / 1000000.0;
    double min_time_ms = min_time / 1000000.0;
    double max_time_ms = max_time / 1000000.0;

    // Calculate throughput
    double avg_throughput_mb_s =
        (file_size / (1024.0 * 1024.0)) / (avg_time / 1000000000.0);

    printf("\n=== Benchmark Results ===\n");
    printf("Iterations:     %d\n", iterations);
    printf("File size:      %zu bytes (%.2f MB)\n", file_size,
           file_size / (1024.0 * 1024.0));
    printf("\nTiming (milliseconds):\n");
    printf("  Average:      %.3f ms\n", avg_time_ms);
    printf("  Minimum:      %.3f ms\n", min_time_ms);
    printf("  Maximum:      %.3f ms\n", max_time_ms);
    printf("  Range:        %.3f ms\n", max_time_ms - min_time_ms);
    printf("\nThroughput:\n");
    printf("  Average:      %.2f MB/s\n", avg_throughput_mb_s);
    printf("  Parse rate:   %.0f parses/second\n", 1000.0 / avg_time_ms);
}

i32 main(i32 argc, char *argv[]) {
    const char *filename = "out.json";
    i32 iterations = 7;

    // Parse command line arguments
    if (argc >= 2) {
        filename = argv[1];
    }
    if (argc >= 3) {
        iterations = atoi(argv[2]);
        if (iterations <= 0) {
            fprintf(stderr, "Error: Invalid number of iterations\n");
            return 1;
        }
    }

    printf("JSON Parser Benchmark\n");
    printf("====================\n");

    run_benchmark(filename, iterations);

    return 0;
}
