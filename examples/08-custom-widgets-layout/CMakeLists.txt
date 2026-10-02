cmake_minimum_required(VERSION 3.20)

project(08-custom-widgets-layout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(widgets_layout_app WIN32
    main.cpp
)

target_compile_definitions(widgets_layout_app PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(widgets_layout_app PRIVATE
    user32
    gdi32
    dwmapi
    msimg32 # Diperlukan untuk fungsi GradientFill
)
