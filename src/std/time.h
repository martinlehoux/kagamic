#pragma once

#include "types.h"

typedef struct {
    u64 s;
    u64 ns;
} Time;

typedef struct {
    u64 s;
    u64 ns;
} Duration;

Time Time_get();
f32 Time_ms(Time time);
Duration Time_diff(Time end, Time start);
f32 Duration_ms(Duration duration);
Duration Duration_from_ms(f32 ms);
Duration Duration_diff(Duration a, Duration b);
