cmake_minimum_required(VERSION 3.30.5)
project(kagamic VERSION 0.1 LANGUAGES C)
set(CMAKE_C_STANDARD 17)

file(GLOB SOURCES "src/*.c")
file(GLOB TEST_SOURCES "test/*.c")

add_compile_options(-Wall -pedantic -Wextra -march=native)

add_executable(main main.c ${SOURCES})
target_link_libraries(main m)
target_compile_options(main PRIVATE)

add_executable(test test.c ${SOURCES} ${TEST_SOURCES})
target_link_libraries(test m)
target_compile_options(test PRIVATE)
