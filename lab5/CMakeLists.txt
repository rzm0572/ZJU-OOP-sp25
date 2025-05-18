cmake_minimum_required(VERSION 3.15)
project(lab5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lab5 "src/main.cpp;src/fraction.cpp")
set_target_properties(lab5 PROPERTIES CXX_STANDARD 17)
target_compile_options(lab5 PUBLIC -Wall -g -O0)
target_include_directories(lab5 PUBLIC include)
