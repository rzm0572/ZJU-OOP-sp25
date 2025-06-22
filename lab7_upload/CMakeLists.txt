cmake_minimum_required(VERSION 3.15)
project(lab7)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(test_stdallocator "test/testallocator.cpp")
set_target_properties(test_stdallocator PROPERTIES CXX_STANDARD 17)
target_compile_options(test_stdallocator PUBLIC -Wall -g -O0)
target_include_directories(test_stdallocator PUBLIC include)

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
