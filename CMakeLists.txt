cmake_minimum_required(VERSION 3.30.5)
project(kagamic VERSION 0.1 LANGUAGES C)
set(CMAKE_C_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/**/*.c")
list(FILTER SOURCES EXCLUDE REGEX ".*/test_.*\\.c$")
list(FILTER SOURCES EXCLUDE REGEX ".*/bench_.*\\.c$")


add_compile_options(-Wall -pedantic -Wextra -mavx2 -mno-avx512f)

add_executable(main main.c ${SOURCES})
target_link_libraries(main m)
target_compile_options(main PRIVATE)

add_executable(test test.c ${SOURCES})
target_link_libraries(test m)
target_compile_options(test PRIVATE)

add_executable(bench bench.c ${SOURCES})
target_link_libraries(bench m)
target_compile_options(bench PRIVATE)
