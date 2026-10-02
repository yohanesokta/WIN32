cmake_minimum_required(VERSION 3.20)

project(Win32Tutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gunakan Unicode secara konsisten di semua target Win32
add_compile_definitions(UNICODE _UNICODE)

# Subdirektori contoh kode (examples)
add_subdirectory(examples)