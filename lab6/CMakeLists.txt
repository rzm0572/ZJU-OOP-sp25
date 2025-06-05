cmake_minimum_required(VERSION 3.15)
project(lab6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lab6 "src/main.cpp")
set_target_properties(lab6 PROPERTIES CXX_STANDARD 17)
target_compile_options(lab6 PUBLIC -Wall -g -O2)
target_include_directories(lab6 PUBLIC include)
