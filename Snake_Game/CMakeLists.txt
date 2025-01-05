cmake_minimum_required(VERSION 3.30)
project(Snake_Game C)

set(CMAKE_C_STANDARD 11)

add_executable(Snake_Game main.c
        game.c
        game.h
        menu.c
        menu.h)
