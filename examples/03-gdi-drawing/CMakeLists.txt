cmake_minimum_required(VERSION 3.20)

project(03-gdi-drawing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gdi_app WIN32
    main.cpp
)

target_compile_definitions(gdi_app PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(gdi_app PRIVATE
    user32
    gdi32
)
