cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-g -Wall)
include_directories(include)

add_executable(lab2 "src/main.cpp;src/student.cpp")
