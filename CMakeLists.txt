cmake_minimum_required(VERSION 4.0)
project(PowderSimulator C)

set(CMAKE_C_STANDARD 23)

add_executable(PowderSimulator src/main.c
        src/PowderSimulator.c
        include/PowderSimulator.h
        src/Window.c
        include/Window.h)

set_target_properties(PowderSimulator PROPERTIES
        WIN32_EXECUTABLE TRUE
)