#include "testing.h"
#include "vec.h"

i32 test_int_vec() {
    Arena perm = Arena_new(128e3);

    Vec vec = Vec_new(&perm, i32, 2);
    i32 *tmp = new(&perm, i32, 1);
    *tmp = 1;
    Vec_push(&perm, &vec, tmp);
    *tmp = 2;
    Vec_push(&perm, &vec, tmp);

    t_assert_equal(*Vec_get(vec, i32, 0), 1);
    t_assert_equal(*Vec_get(vec, i32, 1), 2);
    t_assert_equal(vec.cap, 2);

    *tmp = 3;
    Vec_push(&perm, &vec, tmp);

    t_assert_equal(*Vec_get(vec, i32, 2), 3);
    t_assert_equal(vec.cap, 4);

    return 0;
}
