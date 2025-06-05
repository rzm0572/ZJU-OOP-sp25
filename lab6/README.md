# Lab 6: Vector

## Compile

Make sure you have `g++`, `make` and `cmake` in your system. Then, run the following commands to compile the program:

```bash
./compile.sh
```

The compiled program `lab6` will be generated in the `build` directory.

## Test

The test case is written in the `main.cpp` file. You can run the program to test the `Vector` class:

```bash
./test.sh
```

The output should be:

```
Running tests...
1. Testing Vector<int>...
Success: Vector<int>::push_back() and operator[]() passed
Caught exception: Index out of range
Caught exception: Index out of range
Success: Vector<int>::at() passed
Success: Vector<int>::clear() and empty() passed
Success: Vector<int> copy constructor passed
Success: Vector<int> passed all tests
2. Testing Vector<double>...
Success: Vector<double>::constructor(int size) passed
Success: Vector<double>::at(), operator[]()
Success: Vector<double> passed all tests
3. Testing Vector<TestClass>...
Success: Vector<TestClass>::at() and push_back() passed
Success: Vector<TestClass> passed all tests
Success: All tests passed!
```
