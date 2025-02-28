#!/bin/bash

cmake -B build
cmake --build build

./build/lab1 < test/case1.in
