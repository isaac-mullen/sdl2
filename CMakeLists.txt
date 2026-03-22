cmake_minimum_required(VERSION 4.1)
project(sdl2 C)

set(CMAKE_C_STANDARD 23)

find_package(SDL2 REQUIRED)

add_executable(app main.c)

target_link_libraries(app SDL2::SDL2)
