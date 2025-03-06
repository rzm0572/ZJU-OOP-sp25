#!/bin/bash

cmake -B build
cmake --build build

./build/lab2 < test/case$1.in > test/case$1.out
