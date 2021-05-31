cmake_minimum_required(VERSION 3.19)
project(hacksx C)

set(CMAKE_C_STANDARD 11)

include_directories(header)

add_executable(hacksx
        header/FileReader.h
        header/JSONParser.h
        header/CdReader.h
        header/Map.h
        header/Tiled.h
        header/Logger.h
        header/MemUtils.h
        header/StrUtils.h
        header/MockPSXTypes.h
        header/GPUBase.h
        header/GameObject.h
        header/AssetManager.h
        source/CdReader.c
        source/Map.c
        source/FileReader.c
        source/Main.c
        source/JSONParser.c
        source/Tiled.c
        source/GPUBase.c
        source/GameObject.c
        source/AssetManager.c
        source/Logger.c source/MockPSXTypes.c source/TileFetcher.c header/TileFetcher.h)
