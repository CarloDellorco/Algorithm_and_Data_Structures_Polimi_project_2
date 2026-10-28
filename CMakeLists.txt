cmake_minimum_required(VERSION 3.20)

project(Algorithm_project_2023 VERSION 1.0.0 LANGUAGES C)

add_executable(Algorithm_project_2023 main.c)

set_target_properties(Algorithm_project_2023 PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Algorithm_project_2023 PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()