cmake_minimum_required(VERSION 3.20)

project(01-hello-window LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# WIN32 memberitahu linker untuk menggunakan subsystem:windows (bukan console)
add_executable(hello_window WIN32
    main.cpp
)

target_compile_definitions(hello_window PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(hello_window PRIVATE
    user32
    gdi32
)
