cmake_minimum_required(VERSION 3.19)
project(hacksx C)

set(CMAKE_C_STANDARD 11)

include_directories(header)

add_executable(hacksx
        header/FileReader.h
        header/JSONParser.h
        header/Tiled.h
        header/Utils.h
        source/FileReader.c
        source/Main.c
        source/JSONParser.c
        source/Tiled.c
        header/Logger.h
        header/MemUtils.h
        header/StrUtils.h
        source/Logger.c)
