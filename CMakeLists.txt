cmake_minimum_required(VERSION 3.16)
project(hacksx C)

set(CMAKE_C_STANDARD 11)

include_directories(header psyq)

add_executable(hacksx
        # Standard header files
        header/FileReader.h
        header/JSONParser.h
        header/CdReader.h
        header/Map.h
        header/Tiled.h
        header/Logger.h
        header/MemUtils.h
        header/StrUtils.h
        header/GPUBase.h
        header/GameObject.h
        header/AssetManager.h
        header/TileFetcher.h
        header/Frames.h
        header/TxtHandler.h
        header/LinkedListTutorial.h
        header/SoundEffect.h

        # Mocked psyq header files
        psyq/SYS/TYPES.h
        psyq/LIBETC.h
        psyq/LIBGPU.h
        psyq/LIBGS.h
        psyq/LIBGTE.h
        psyq/LIBDS.c
        psyq/LIBSPU.h
        psyq/MockPSXTypes.c
        psyq/LIBDS.h
        header/Timer.h
        source/Timer.c

        # Standard sources
        source/LinkedListTutorial.c
        source/Frames.c
        source/CdReader.c
        source/Map.c
        source/FileReader.c
        source/Main.c
        source/JSONParser.c
        source/Tiled.c
        source/GPUBase.c
        source/GameObject.c
        source/AssetManager.c
        source/Logger.c
        source/TxtHandler.c
        source/TileFetcher.c

        )
