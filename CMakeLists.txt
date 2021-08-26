cmake_minimum_required(VERSION 3.16)
project(RanSanMoi)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(RanSanMoi
        kmin_console.cpp
        kmin_console.h
        main.cpp
        resource.h)
