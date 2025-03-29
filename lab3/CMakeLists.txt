cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(lab3 "src/main.cpp;src/game.cpp;src/castle.cpp;src/disjoint_set.cpp")
