cmake_minimum_required(VERSION 3.5)
project(train_simulation)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c var.h
        input.h output.h state_trans.h control.h
        input.c output.c state_trans.c control.c)

add_executable(train_simulation ${SOURCE_FILES})