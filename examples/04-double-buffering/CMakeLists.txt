cmake_minimum_required(VERSION 3.20)

project(04-double-buffering LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(double_buffer_app WIN32
    main.cpp
)

target_compile_definitions(double_buffer_app PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(double_buffer_app PRIVATE
    user32
    gdi32
)
