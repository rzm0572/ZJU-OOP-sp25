#!/bin/bash

GREEN="\033[32m"
GREENBOLD="\033[1;32m"
NC="\033[0m"

echo_and_run() {
    echo -e "${GREEN}[Running] $*${NC}"
    eval "$*"
}

# Build the program
echo -e "${GREENBOLD}Compiling...${NC}"
./compile.sh

# Run the test cases
echo -e "${GREENBOLD}Running test cases...${NC}"

cd test

# Test case 1: add diary entries
echo_and_run ./pdadd < data/diary1.txt
echo_and_run ./pdadd < data/diary2.txt
echo_and_run ./pdadd < data/diary3.txt

# Test case 2: show diary entries
echo_and_run ./pdshow 2025-01-08                 # single date from command line
echo_and_run ./pdshow 2025-01-08 2025-01-15      # multiple dates from command line

# Test case 3: list diary entries
echo_and_run ./pdlist 2025-01-01 2025-01-31      # date range from command line
echo_and_run ./pdlist 2025-01-14 2025-02-22

echo -e "${GREEN}[Running] ./pdlist 2025-01-01 2025-01-31 | ./pdshow${NC}"
./pdlist 2025-01-01 2025-01-31 | ./pdshow    # test piping

# Test case 4: remove diary entries
echo_and_run ./pdremove 2025-01-15
echo_and_run ./pdshow 2025-01-15                 # should be empty
echo_and_run ./pdshow 2025-01-08
echo_and_run ./pdlist 2025-01-01 2025-01-31

# Test case 5: remove all diary entries
echo_and_run ./pdremove 2025-01-15               # remove not-existed date
echo_and_run ./pdremove 2025-01-08               # remove the last date
echo_and_run ./pdremove < data/date1.txt         # date from stdin
echo_and_run ./pdshow 2025-01-15                 # should be empty
echo_and_run ./pdlist 2025-01-01 2025-01-31      # should be empty

cd ..

echo -e "${GREENBOLD}All test cases passed!${NC}"
