cmake_minimum_required(VERSION 3.20)
project(lab4_txt)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -g -O0)

include_directories(include)

add_executable(pdadd "src/pdadd.cpp;lib/diary_manager.cpp;lib/structures.cpp")
add_executable(pdlist "src/pdlist.cpp;lib/diary_manager.cpp;lib/structures.cpp")
add_executable(pdshow "src/pdshow.cpp;lib/diary_manager.cpp;lib/structures.cpp")
add_executable(pdremove "src/pdremove.cpp;lib/diary_manager.cpp;lib/structures.cpp")
