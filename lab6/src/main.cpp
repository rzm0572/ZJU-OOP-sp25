// This is the test code for the Vector<T> class.
// We will conduct difftest between our Vector<T> and std::vector<T>.

#include "vector.h"
#include <_stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <vector>
#include <assert.h>

// Define color macros for console output
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_CYAN "\033[36m"
#define COLOR_RESET "\033[0m"

// Define message macros for console output
#define ERROR_MSG(msg) COLOR_RED "Error: "  msg  COLOR_RESET
#define SUCCESS_MSG(msg) COLOR_GREEN "Success: " msg  COLOR_RESET
#define INFO_MSG(msg) COLOR_CYAN msg COLOR_RESET

// Define ASSERT macro for test assertions
#define ASSERT(expr, message) assert((expr) && (message))

// Random generator class to generate random integers, doubles, and strings
class RandomGenerator {
public:
    // Generate a random integer between low and high (inclusive)
    static int RandomInt(int low, int high) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int64_t> dist(low, high);
        return dist(gen);
    }

    // Generate a random double between low and high (inclusive)
    static double RandomDouble(double low, double high) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(low, high);
        return dist(gen);
    }

    // Generate a random string of length length
    static std::string RandomString(int length) {
        char *buf = new char[length];
        for (int i = 0; i < length; i++) {
            buf[i] = RandomInt(33, 122);
        }
        return std::string(buf, length);
    }
};

// The class is used to test Vector<TestClass>
class TestClass {
private:
    int int_field;
    double double_field;
    std::string string_field;
public:
    TestClass(int int_field = 0, double double_field = 0.0, std::string string_field = "") : int_field(int_field), double_field(double_field), string_field(string_field) {}
    
    TestClass(const TestClass& other) : int_field(other.int_field), double_field(other.double_field), string_field(other.string_field) {}

    bool operator==(const TestClass& other) const {
        return int_field == other.int_field && double_field == other.double_field && string_field == other.string_field;
    }

    TestClass& operator=(const TestClass& other) {
        if (this != &other) {
            int_field = other.int_field;
            double_field = other.double_field;
            string_field = other.string_field;
        }
        return *this;
    }

    // Randomize the fields of the object
    void Randomize() {
        int_field = RandomGenerator::RandomInt(0, 1000000);
        double_field = RandomGenerator::RandomDouble(0.0, 1000000.0);
        string_field = RandomGenerator::RandomString(128);
    }
};

int main() {
    int N = 10000;

    std::cout << INFO_MSG("Running tests...") << std::endl;

    // Test Vector<int>
    std::cout << INFO_MSG("1. Testing Vector<int>...") << std::endl;

    // Test default constructor of Vector<int>
    Vector<int> v1;
    std::vector<int> std_v1;

    // Randomly push N integers into the vector and std::vector
    for (int i = 0; i < N; i++) {
        int random_int = RandomGenerator::RandomInt(0, 1000000);
        v1.push_back(random_int);
        std_v1.push_back(random_int);
    }

    // Check if the size and elements of the vectors are equal
    ASSERT(v1.size() == N, ERROR_MSG("Size mismatch"));
    ASSERT(v1.size() == std_v1.size(), ERROR_MSG("Size mismatch"));

    // Check if the elements of the vectors are equal
    for (int i = 0; i < N; i++) {
        ASSERT(v1[i] == std_v1[i], ERROR_MSG("Vectors are not equal"));
    }

    std::cout << SUCCESS_MSG("Vector<int>::push_back() and operator[]() passed") << std::endl;

    // Test Vector<int>::at() [in-range case]
    try {
        for (int i = 0; i < N; i++) {
            ASSERT(v1.at(i) == std_v1.at(i), ERROR_MSG("Vectors are not equal"));
        }
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
        ASSERT(false, ERROR_MSG("Should not have thrown exception"));
    }

    // Test Vector<int>::at() [out-of-range case]
    try {
        v1.at(-1);
        ASSERT(false, ERROR_MSG("Should have thrown exception"));
    } catch(const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    // Test Vector<int>::at() [out-of-range case]
    try {
        v1.at(RandomGenerator::RandomInt(N, 2*N));
        ASSERT(false, ERROR_MSG("Should have thrown exception"));
    } catch(const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    std::cout << SUCCESS_MSG("Vector<int>::at() passed") << std::endl;

    Vector<int> v2(v1);

    // Test Vector<int>::clear() and Vector<int>::empty()
    ASSERT(!v1.empty(), ERROR_MSG("Vector should not be empty"));
    v1.clear();
    ASSERT(v1.empty(), ERROR_MSG("Vector should be empty"));

    std::cout << SUCCESS_MSG("Vector<int>::clear() and empty() passed") << std::endl;

    // Test the copy constructor of Vector<int>. Check if the elements of the vectors are equal.
    for (int i = 0; i < N; i++) {
        ASSERT(v2[i] == std_v1[i], ERROR_MSG("Vectors are not equal"));
    }

    std::cout << SUCCESS_MSG("Vector<int> copy constructor passed") << std::endl;

    v2.clear();
    ASSERT(v2.empty(), ERROR_MSG("Vector should be empty"));

    std::cout << SUCCESS_MSG("Vector<int> passed all tests") << std::endl;

    // Test Vector<double>
    std::cout << INFO_MSG("2. Testing Vector<double>...") << std::endl;

    // Test Vector<double>::constructor(int size)
    Vector<double> v3(N);
    std::vector<double> std_v3(N);

    // Test Vector<double>::at() as left value
    try {
        for (int i = 0; i < N; i++) {
            double random_double = RandomGenerator::RandomDouble(0.0, 1000000.0);

            v3.at(i) = random_double;
            std_v3[i] = random_double;
        }
    } catch (const std::out_of_range& e) {
        ASSERT(false, ERROR_MSG("Should not have thrown exception"));
    }

    std::cout << SUCCESS_MSG("Vector<double>::constructor(int size) passed") << std::endl;

    // Check if the size and elements of the vectors are equal
    ASSERT(v3.size() == N, ERROR_MSG("Size mismatch"));
    ASSERT(v3.size() == std_v3.size(), ERROR_MSG("Size mismatch"));

    // Check if the elements of the vectors are equal
    try {
        for (int i = 0; i < N; i++) {
            ASSERT(v3.at(i) == std_v3.at(i), ERROR_MSG("Vectors are not equal"));
        }
    } catch (const std::out_of_range& e) {
        ASSERT(false, ERROR_MSG("Should not have thrown exception"));
    }

    std::cout << SUCCESS_MSG("Vector<double>::at(), operator[]()") << std::endl;

    // Test Vector<double>::clear() and Vector<double>::empty()
    ASSERT(!v3.empty(), ERROR_MSG("Vector should not be empty"));
    v3.clear();
    std_v3.clear();
    ASSERT(v3.empty(), ERROR_MSG("Vector should be empty"));

    std::cout << SUCCESS_MSG("Vector<double> passed all tests") << std::endl;

    // Test Vector<TestClass>
    std::cout << INFO_MSG("3. Testing Vector<TestClass>...") << std::endl;

    Vector<TestClass> v4;
    std::vector<TestClass> std_v4;

    // Test Vector<TestClass>::push_back()
    for (int i = 0; i < N; i++) {
        TestClass tc;
        tc.Randomize();
        v4.push_back(tc);
        std_v4.push_back(tc);
    }

    // Check if the size and elements of the vectors are equal
    ASSERT(v4.size() == N, ERROR_MSG("Size mismatch"));

    // Check if the elements of the vectors are equal
    try {
        for (int i = 0; i < N; i++) {
            ASSERT(v4.at(i) == std_v4.at(i), ERROR_MSG("Vectors are not equal"));
        }
    } catch (const std::out_of_range& e) {
        ASSERT(false, ERROR_MSG("Should not have thrown exception"));
    }

    std::cout << SUCCESS_MSG("Vector<TestClass>::at() and push_back() passed") << std::endl;

    // free the memory of the vectors
    v4.clear();
    std_v4.clear();

    std::cout << SUCCESS_MSG("Vector<TestClass> passed all tests") << std::endl;

    std::cout << SUCCESS_MSG("All tests passed!") << std::endl;

    return 0;
}
