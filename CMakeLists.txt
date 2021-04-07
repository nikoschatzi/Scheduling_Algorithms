cmake_minimum_required(VERSION 3.17)
project(Scheduling C)

set(CMAKE_C_STANDARD 99)

add_executable(Scheduling main.c FCFS.h PP.h SRTF.h RR.h)