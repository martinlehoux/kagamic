#include "testing.h"
#include "vec.h"

typedef struct {
    i32 x;
    i32 y;
} Point;

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

i32 test_vec_push() {
    Arena perm = Arena_new(128e3);
    Vec v = Vec_new(&perm, i32, 4);

    i32 vals[] = {10, 20, 30};
    for (i32 i = 0; i < 3; i++) Vec_push(&perm, &v, &vals[i]);

    t_assert_equal(v.len, 3);
    t_assert_equal(*Vec_get(v, i32, 0), 10);
    t_assert_equal(*Vec_get(v, i32, 1), 20);
    t_assert_equal(*Vec_get(v, i32, 2), 30);
    t_assert_equal(Arena_get_used(perm), v.cap * sizeof(i32));

    return 0;
}

i32 test_vec_grow() {
    Arena perm = Arena_new(128e3);
    Vec v = Vec_new(&perm, i32, 2);

    for (i32 i = 0; i < 8; i++) Vec_push(&perm, &v, &i);

    t_assert_equal(v.len, 8);
    t_assert_equal(Arena_get_used(perm), sizeof(i32) * (2 + 4 + 8));

    return 0;
}

i32 test_vec_struct() {
    Arena perm = Arena_new(128e3);
    Vec v = Vec_new(&perm, Point, 2);

    Point p1 = {1, 2}, p2 = {3, 4};
    Vec_push(&perm, &v, &p1);
    Vec_push(&perm, &v, &p2);

    Point *r0 = Vec_get(v, Point, 0);
    Point *r1 = Vec_get(v, Point, 1);
    t_assert_equal(r0->x, 1);
    t_assert_equal(r0->y, 2);
    t_assert_equal(r1->x, 3);
    t_assert_equal(r1->y, 4);
    t_assert_equal(Arena_get_used(perm), 16);

    return 0;
}

i32 test_vec_initial_len() {
    Arena perm = Arena_new(128e3);
    Vec v = Vec_new(&perm, i32, 4);

    t_assert_equal(v.len, 0);
    t_assert_equal(v.cap, 4);
    t_assert_equal(Arena_get_used(perm), 16);

    return 0;
}
