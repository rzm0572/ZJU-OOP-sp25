cmake_minimum_required(VERSION 3.10)
project(lab1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-g -Wall)
include_directories(include)

add_executable(lab1 "src/main.cpp;src/student.cpp")
