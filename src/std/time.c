#include <time.h>

#include "assert.h"
#include "time.h"
#include "types.h"

Time Time_get() {
    struct timespec system_time;
    assert(clock_gettime(CLOCK_MONOTONIC, &system_time) == 0,
           "clock_gettime failed");
    return (Time){system_time.tv_sec, system_time.tv_nsec};
}

f32 Time_ms(Time time) { return time.s * 1e3 + time.ns / 1e6; }

Duration Time_diff(Time end, Time start) {
    Duration d;
    d.s = end.s - start.s;
    d.ns = end.ns - start.ns;
    if (d.ns >= (u64)1e9) {
        d.s += d.ns / (u64)1e9;
        d.ns = d.ns % (u64)1e9;
    }
    return d;
}

f32 Duration_ms(Duration d) { return d.s * 1e3 + d.ns / 1e6; }

Duration Duration_from_ms(f32 ms) {
    Duration d;
    d.s = (u64)(ms / 1e3);
    d.ns = (u64)((ms - d.s * 1e3) * 1e6);
    return d;
}

Duration Duration_diff(Duration a, Duration b) {
    Duration d;
    d.s = a.s - b.s;
    d.ns = a.ns - b.ns;
    if (d.ns >= (u64)1e9) {
        d.s += d.ns / (u64)1e9;
        d.ns = d.ns % (u64)1e9;
    }
    return d;
}
