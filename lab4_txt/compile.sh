#!/bin/bash

set -e

cmake -B build
cmake --build build

mkdir -p test
cp build/pd* test/
