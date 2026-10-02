cmake_minimum_required(VERSION 3.20)

project(05-dpi-aware LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dpi_app WIN32
    main.cpp
)

target_compile_definitions(dpi_app PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(dpi_app PRIVATE
    user32
    gdi32
)
