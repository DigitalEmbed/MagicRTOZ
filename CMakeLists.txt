cmake_minimum_required(VERSION 3.21)
project(MagicRTOZ C)

set(CMAKE_C_STANDARD 11)

include_directories(MagicRTOZ)

add_executable(MagicRTOZ
        MagicRTOZ/config.h
        MagicRTOZ/MagicRTOZ.h
        MagicRTOZ/process.c
        MagicRTOZ/process.h
        MagicRTOZ/slist.c
        MagicRTOZ/slist.h
        MagicRTOZ/stimer.c
        MagicRTOZ/stimer.h
        MagicRTOZ/system.c
        MagicRTOZ/task.c
        MagicRTOZ/task.h
        main.c)
