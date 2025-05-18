# Lab 5: Fraction

## Compile

Make sure you have `g++`, `make` and `cmake` in your system. Then, run the following commands to compile the program:

```bash
./compile.sh
```

The compiled program `lab5` will be generated in the `build` directory.

## Test

There are one test case in the `test` directory, including the test cases for all the methods of the `Fraction` class.

To test the program, run the following command:

```bash
./run_test.sh
```

The test cases will be run and the results will be printed to the console.

## Usage

The program `build/lab5` is a command-line program that test the `Fraction` class.

The program will first take an integer as test type:

- 0: Test the arithmetical operations of `Fraction` class.
- 1: Test the comparison operations of `Fraction` class.
- 2: Test the input from stream.
- 3: Test the output to stream.
- 4: Test the conversion from `Fraction` to `double` and `std::string`.
- 5: Test the conversion from `std::string` (with fraction or decimal format) to `Fraction`.

Then, it will take input parameters according to the test type.

For example, if you want to test the arithmetical operations of `Fraction` class, you can input:

```bash
0 1/2 1/3
```

The output will be:

```
[Test #0 for type 0]
1/2 + 1/3 = 5/6
1/2 - 1/3 = 1/6
1/2 * 1/3 = 1/6
1/2 / 1/3 = 3/2
```


