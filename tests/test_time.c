#include "../include/testing.h"
#include "../include/time.h"
#include "../include/types.h"

i32 test_time_diff() {
    Time start = Time_get();
    Time end = Time_get();

    Duration diff = Time_diff(end, start);
    t_assert_equal(diff.s, 0);

    return 0;
}
