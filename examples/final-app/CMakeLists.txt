cmake_minimum_required(VERSION 3.20)

project(final-app LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(modern_app WIN32
    main.cpp
    app.rc
)

target_compile_definitions(modern_app PRIVATE
    UNICODE
    _UNICODE
)

target_link_libraries(modern_app PRIVATE
    user32
    gdi32
    dwmapi
)
