cmake_minimum_required(VERSION 3.9)
project(bomberman_server C)

set(CMAKE_C_STANDARD 90)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(bomberman_server main.c net.h net.c game.h input.h input.c game.c libs.h)
add_executable(test_server test_server.c)

target_link_libraries(bomberman_server ${CURSES_LIBRARIES})