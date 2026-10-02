cmake_minimum_required(VERSION 3.20)

project(02-input-keyboard-mouse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(input_app WIN32
    main.cpp
)

target_compile_definitions(input_app PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(input_app PRIVATE
    user32
    gdi32
)
