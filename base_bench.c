#include <math.h>
#include <stdio.h>
#include <string.h>

#include "src/std/memory.h"
#include "src/std/time.h"
#include "src/std/types.h"

#define BENCH_DURATION_MS 100.0f

typedef struct {
    f32 ns;
    f32 stddev;
    size peak;
    size net;
    f32 total_ms;
} BenchResult;

typedef void (*BenchSetup)(Arena *a);
typedef i32 (*BenchFn)(Arena *a);

static inline BenchResult bench_run(BenchSetup setup, BenchFn fn) {
    Arena scratch = Arena_new(32 * 1024 * 1024);
    u64 calls = 0;
    size peak = 0;
    size net = 0;
    double mean = 0, m2 = 0;
    double total_ns = 0;

    Time wall_start = Time_get();
    Duration limit = Duration_from_ms(BENCH_DURATION_MS);
    u64 limit_ns = limit.ns + limit.s * (u64)1e9;

    for (;;) {
        Time wall_now = Time_get();
        Duration wall_elapsed = Time_diff(wall_now, wall_start);
        if (wall_elapsed.s > 0 ||
            wall_elapsed.ns >= limit_ns)
            break;

        Arena_reset(&scratch);
        setup(&scratch);
        size before = Arena_get_used(scratch);
        Time start = Time_get();
        fn(&scratch);
        Time end = Time_get();
        size after = Arena_get_used(scratch);
        Duration d = Time_diff(end, start);
        double sample = (double)d.ns + (double)d.s * 1e9;
        total_ns += sample;
        calls++;
        double delta = sample - mean;
        mean += delta / (double)calls;
        m2 += delta * (sample - mean);
        if (after > peak) peak = after;
        net = after - before;
    }

    if (calls == 0) calls = 1;
    return (BenchResult){
        .ns = (f32)(total_ns / (double)calls),
        .stddev = calls > 1 ? (f32)sqrt(m2 / (double)(calls - 1)) : 0,
        .peak = peak,
        .net = net,
        .total_ms = (f32)(total_ns / 1e6),
    };
}

#define MAX_RESULTS 64
typedef struct {
    const char *name;
    BenchResult result;
} BenchEntry;
static BenchEntry _bench_registry[MAX_RESULTS];
static i32 _bench_count = 0;

static void _bench_register(const char *name, BenchResult r) {
    if (_bench_count < MAX_RESULTS) {
        _bench_registry[_bench_count].name = name;
        _bench_registry[_bench_count].result = r;
        _bench_count++;
    }
}

static BenchResult *_bench_find(const char *name) {
    for (i32 i = 0; i < _bench_count; i++)
        if (strcmp(_bench_registry[i].name, name) == 0)
            return &_bench_registry[i].result;
    return NULL;
}

static void _fmt_mem(char *buf, size_t bufsz, size bytes) {
    if (bytes >= 1024 * 1024)
        snprintf(buf, bufsz, "%.1f MB", (double)bytes / (1024 * 1024));
    else if (bytes >= 1024)
        snprintf(buf, bufsz, "%.1f KB", (double)bytes / 1024);
    else
        snprintf(buf, bufsz, "%zu B", bytes);
}

static void _fmt_ratio(char *buf, size_t bufsz, double val, double ref) {
    if (ref == 0) {
        buf[0] = '\0';
        return;
    }
    double ratio = val / ref;
    if (ratio >= 1.005)
        snprintf(buf, bufsz, "(x%.0f)", ratio);
    else if (ratio < 0.995)
        snprintf(buf, bufsz, "(x%.2f)", ratio);
    else
        snprintf(buf, bufsz, "(x1)");
}

static void _bench_print_row(const char *name, BenchResult r,
                              const char *compare) {
    BenchResult *ref = compare ? _bench_find(compare) : NULL;

    char ns_ratio[16] = "", peak_ratio[16] = "", net_ratio[16] = "";
    if (ref) {
        _fmt_ratio(ns_ratio, sizeof(ns_ratio), r.ns, ref->ns);
        _fmt_ratio(peak_ratio, sizeof(peak_ratio), (double)r.peak,
                   (double)ref->peak);
        _fmt_ratio(net_ratio, sizeof(net_ratio), (double)r.net,
                   (double)ref->net);
    }

    char peak[16], net[16];
    _fmt_mem(peak, sizeof(peak), r.peak);
    _fmt_mem(net, sizeof(net), r.net);

    printf("  %-38s %10.0f ±%7.0f %-6s   %10s %-6s   %10s %-6s   %6.0f ms\n",
           name, r.ns, r.stddev, ns_ratio, peak, peak_ratio, net, net_ratio,
           r.total_ms);
}

static void bench_group(const char *label) {
    printf("\n[%s]\n", label);
    printf("  %-38s %10s %-14s   %10s %-6s   %10s %-6s   %9s\n", "name",
           "ns/call", "± stddev", "peak mem", "", "net mem", "", "total");
}

#define bench(name, setup, func, compare)                                      \
    do {                                                                       \
        BenchResult _r = bench_run(setup, func);                               \
        _bench_register(name, _r);                                             \
        _bench_print_row(name, _r, compare);                                   \
    } while (0)
