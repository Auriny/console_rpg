cmake_minimum_required(VERSION 4.0)
project(console_rpg C)

set(CMAKE_C_STANDARD 11)

add_executable(console_rpg main.c
        Player.c
        battle.c
        utils.c
        utils.h
        player.h
        battle.h)
