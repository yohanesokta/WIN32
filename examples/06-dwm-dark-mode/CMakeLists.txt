cmake_minimum_required(VERSION 3.20)

project(06-dwm-dark-mode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dwm_app WIN32
    main.cpp
)

target_compile_definitions(dwm_app PRIVATE
    UNICODE
    _UNICODE
)

# Wajib menghubungkan dwmapi untuk fungsi DwmSetWindowAttribute
target_link_libraries(dwm_app PRIVATE
    user32
    gdi32
    dwmapi
)
