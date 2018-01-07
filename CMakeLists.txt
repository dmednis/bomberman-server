cmake_minimum_required(VERSION 3.9)
project(bomberman_server C)

set(CMAKE_C_STANDARD 90)

add_executable(bomberman_server main.c net.h net.c game.h input.h input.c game.c player.c player.h)
add_executable(test_server test_server.c)