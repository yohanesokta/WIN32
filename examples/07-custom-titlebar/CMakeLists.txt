cmake_minimum_required(VERSION 3.20)

project(07-custom-titlebar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(custom_titlebar_app WIN32
    main.cpp
)

target_compile_definitions(custom_titlebar_app PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(custom_titlebar_app PRIVATE
    user32
    gdi32
    dwmapi
)
