cmake_minimum_required(VERSION 3.10)
project(lab4)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -g -O0)

include_directories(include)

add_executable(pdadd "src/pdadd.cpp;lib/diary.cpp;lib/diary_manager.cpp;lib/page_table.cpp")
add_executable(pdlist "src/pdlist.cpp;lib/diary.cpp;lib/diary_manager.cpp;lib/page_table.cpp")
add_executable(pdshow "src/pdshow.cpp;lib/diary.cpp;lib/diary_manager.cpp;lib/page_table.cpp")
add_executable(pdremove "src/pdremove.cpp;lib/diary.cpp;lib/diary_manager.cpp;lib/page_table.cpp")
