cmake_minimum_required(VERSION 3.19)
project(hacksx C)

set(CMAKE_C_STANDARD 11)

include_directories(header)

add_executable(hacksx
        header/filereader.h
        header/parser.h
        header/tiled.h
        header/utils.h
        source/filereader.c
        source/main.c
        source/parser.c
        source/tiled.c)
