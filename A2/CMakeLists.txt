cmake_minimum_required(VERSION 3.5)
project(A2)

set(A2_SOURCE_FILES
        definitions.h
        resources.h
        vector.c
        vector.h
        carwash.c)

add_compile_options(-Wall -Werror -pthread)
add_executable(carwash ${A2_SOURCE_FILES})
target_link_libraries(carwash pthread)

