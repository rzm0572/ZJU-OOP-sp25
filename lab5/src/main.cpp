#include <iostream>
#include "fraction.h"

// Test functions

// @brief Output the result of arithmetical operations of two fractions.
// @param f1 The first operand.
// @param f2 The second operand.
void test_arithmetical_operations(Fraction f1, Fraction f2) {
    std::cout << f1 << " + " << f2 << " = " << f1 + f2 << std::endl;
    std::cout << f1 << " - " << f2 << " = " << f1 - f2 << std::endl;
    std::cout << f1 << " * " << f2 << " = " << f1 * f2 << std::endl;
    std::cout << f1 << " / " << f2 << " = " << f1 / f2 << std::endl;
}

// @brief Output the result of relational operations of two fractions.
// @param f1 The first operand.
// @param f2 The second operand.
void test_relational_operations(Fraction f1, Fraction f2) {
    std::cout << f1 << " == " << f2 << " : " << (f1 == f2) << std::endl;
    std::cout << f1 << "!= " << f2 << " : " << (f1!= f2) << std::endl;
    std::cout << f1 << " < " << f2 << " : " << (f1 < f2) << std::endl;
    std::cout << f1 << " <= " << f2 << " : " << (f1 <= f2) << std::endl;
    std::cout << f1 << " > " << f2 << " : " << (f1 > f2) << std::endl;
    std::cout << f1 << " >= " << f2 << " : " << (f1 >= f2) << std::endl;
}

// @brief Test for getting input from stream and converting it to Fraction.
Fraction test_input_from_stream() {
    Fraction f;
    std::cin >> f;
    return f;
}

// @brief Test for outputting Fraction to stream.
void test_output_to_stream(Fraction f) {
    std::cout << f << std::endl;
}

// @brief Test for converting Fraction to double and string.
void test_conversion(Fraction f, std::string orig) {
    double d = f.to_double();
    std::cout << orig << " to double = " << d << std::endl;
    std::string str = f.to_string();
    std::cout << orig << " to string = " << str << std::endl;
}

// @brief Test for converting string (fraction or decimal) to Fraction.
void test_conversion_from_string(std::string str) {
    Fraction f;
    f.from_decimal_string(str);
    std::cout << str << " to Fraction = " << f << std::endl;
}

// Test type enum, used to select test cases.
enum TestType {
    TEST_ARITHMETICAL_OPERATIONS,
    TEST_RELATIONAL_OPERATIONS,
    TEST_INPUT_FROM_STREAM,
    TEST_OUTPUT_TO_STREAM,
    TEST_CONVERSION,
    TEST_CONVERSION_FROM_STRING
};

int main() {
    int select = 0, count = 0;
    Fraction f1, f2;
    
    // Input the test type
    // 0: Test arithmetical operations
    // 1: Test relational operations
    // 2: Test input from stream
    // 3: Test output to stream
    // 4: Test conversion
    // 5: Test conversion from string
    // other: Exit
    while (std::cin >> select && select >= 0 && select <= 5) {
        std::cout << "[\033[1;32mTest #" << count << " for type " << select << "\033[0m]" << std::endl;
        switch (select) {
            case TEST_ARITHMETICAL_OPERATIONS: {
                std::cin >> f1 >> f2;
                test_arithmetical_operations(f1, f2);
                break;
            }
            case TEST_RELATIONAL_OPERATIONS: {
                std::cin >> f1 >> f2;
                test_relational_operations(f1, f2);
                break;
            }
            case TEST_INPUT_FROM_STREAM: {
                f1 = test_input_from_stream();
                std::cout << "Input from stream: " << f1 << std::endl;
                break;
            }
            case TEST_OUTPUT_TO_STREAM: {
                std::cin >> f1;
                test_output_to_stream(f1);
                break;
            }
            case TEST_CONVERSION: {
                std::string str;
                std::cin >> str;
                f1.from_string(str);
                test_conversion(f1, str);
                break;
            }
            case TEST_CONVERSION_FROM_STRING: {
                std::string str;
                std::cin >> str;
                test_conversion_from_string(str);
                break;
            }
            default: break;
        }
    }

    return 0;
}
