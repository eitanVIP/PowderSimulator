cmake_minimum_required(VERSION 4.0)
project(PowderSimulator C)

set(CMAKE_C_STANDARD 23)

add_executable(PowderSimulator main.c
        PowderSimulator.c
        PowderSimulator.h
        Window.c
        Window.h)
