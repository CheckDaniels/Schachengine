cmake_minimum_required(VERSION 3.26)
project(06_Chess C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
set(CMAKE_C_STANDARD 23)

add_executable(06_Chess uci.c
        board.c
        search.c
        eval.c
        move_gen.c
        benchmarking.h
        board.h
        uci.h
        search.h
        move_gen.h
        eval.h
        converter.c
        converter.h
        main.c
        benchmarking.c
        makemove.c
        makemove.c
)
