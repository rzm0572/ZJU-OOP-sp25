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

add_executable(test_stdallocator "test/testallocator.cpp")
set_target_properties(test_stdallocator PROPERTIES CXX_STANDARD 17)
target_compile_options(test_stdallocator PUBLIC -Wall -g -O0)
target_include_directories(test_stdallocator PUBLIC include)

add_executable(test_mtallocator "test/testallocator.cpp")
set_target_properties(test_mtallocator PROPERTIES CXX_STANDARD 17)
target_compile_definitions(test_mtallocator PUBLIC MyAllocator=mtAllocator)
target_compile_options(test_mtallocator PUBLIC -Wall -g -O0)
target_include_directories(test_mtallocator PUBLIC include)

add_executable(test_lallocator "test/testallocator.cpp")
set_target_properties(test_lallocator PROPERTIES CXX_STANDARD 17)
target_compile_definitions(test_lallocator PUBLIC MyAllocator=Allocator)
target_compile_options(test_lallocator PUBLIC -Wall -g -O0)
target_include_directories(test_lallocator PUBLIC include)

add_executable(test_pallocator "test/testallocator.cpp")
set_target_properties(test_pallocator PROPERTIES CXX_STANDARD 17)
target_compile_definitions(test_pallocator PUBLIC MyAllocator=pAllocator)
target_compile_options(test_pallocator PUBLIC -Wall -g -O0)
target_include_directories(test_pallocator PUBLIC include)

set(DFT_SLOT
    1
    2
    4
    8
    12
    16
    20
    24
)

foreach(slot IN LISTS DFT_SLOT)
    add_executable(test_mallocator_${slot} "test/testallocator.cpp")
    set_target_properties(test_mallocator_${slot} PROPERTIES CXX_STANDARD 17)
    target_compile_definitions(test_mallocator_${slot} PUBLIC MyAllocator=mAllocator DEFAULT_ALLOC_SLOT=${slot})
    target_compile_options(test_mallocator_${slot} PUBLIC -Wall -g -O0)
    target_include_directories(test_mallocator_${slot} PUBLIC include)
endforeach()
