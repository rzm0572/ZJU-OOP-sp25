cmake_minimum_required(VERSION 3.15)
project(lab7)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main "src/main.cpp")
set_target_properties(main PROPERTIES CXX_STANDARD 17)
target_compile_options(main PUBLIC -Wall -g -O0)
target_include_directories(main PUBLIC include)

add_executable(std "src/std.cpp")
set_target_properties(std PROPERTIES CXX_STANDARD 17)
target_compile_options(std PUBLIC -Wall -g -O0)

add_executable(testallocator "test/testallocator.cpp")
set_target_properties(testallocator PROPERTIES CXX_STANDARD 17)
target_compile_options(testallocator PUBLIC -Wall -g -O0)
target_include_directories(testallocator PUBLIC include)
