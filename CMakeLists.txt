cmake_minimum_required(VERSION 3.30.5)
project(kagamic VERSION 0.1 LANGUAGES C)
set(CMAKE_C_STANDARD 17)
add_executable(main
    main.c
    string.h string.c
    memory.h memory.c
    vec.h vec.c
    json.h json.c
    log.h log.c
)
target_link_libraries(main m)
target_compile_options(main PRIVATE -Wall -pedantic -Wextra)
