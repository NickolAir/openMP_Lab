cmake_minimum_required(VERSION 3.24)
project(openMP C)

set(CMAKE_C_STANDARD 99)

find_package(OpenMP REQUIRED)
link_libraries(m)

add_executable(openMP parallel_for.c parallel_iteration.c)
target_link_libraries(openMP PRIVATE "${OpenMP_C_FLAGS}")
target_compile_options(openMP PRIVATE "${OpenMP_C_FLAGS}")