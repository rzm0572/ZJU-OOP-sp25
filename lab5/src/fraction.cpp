#include "fraction.h"
#include <iostream>
#include <string>

// Helper functions

// @brief Compute the greatest common divisor of a and b using Euclid's algorithm
int gcd(int a, int b) {
    // if b is zero, return a
    // otherwise, continue computing the gcd of b and a % b
    return b == 0 ? a : gcd(b, a % b);
}

// @brief Compute the least common multiple of a and b
int lcm(int a, int b) {
    // lcm(a, b) = a * b / gcd(a, b)
    return a * b / gcd(a, b);
}

// @brief Compute a^exp
int pow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// @brief Compute sgn(x)
inline int sgn(int x) {
    if (x > 0) {
        return 1;
    } else if (x < 0) {
        return -1;
    } else {
        return 0;
    }
}

// @brief Compute |x|
inline int abs(int x) {
    return x >= 0 ? x : -x;
}

// @brief Get the number of digits of integer x
int get_digit_num(int x) {
    int digit_num = 0;
    while (x > 0) {
        x /= 10;
        digit_num++;
    }
    return digit_num;
}

// @brief Reduce the fraction to lowest terms
void Fraction::reduce() {
    if (denominator == 0) {
        throw "Denominator cannot be zero";
    }

    // Get the greatest common divisor of numerator and denominator
    int gcd_val = gcd(numerator, denominator);

    // Divide numerator and denominator by gcd_val
    numerator /= gcd_val;
    denominator /= gcd_val;
}

// Constructors

// Default constructor: Set fraction to 0
Fraction::Fraction(): numerator(0), denominator(1), sign(0) {}

// Constructor with numerator and denominator
// If denominator is zero, throw an exception
// Automatically set the sign of the fraction and reduce it to lowest terms
Fraction::Fraction(int numerator, int denominator) {
    if (denominator == 0) {
        throw "Denominator cannot be zero";
    }
    this->sign = sgn(numerator) * sgn(denominator);
    this->numerator = abs(numerator);
    this->denominator = abs(denominator);
    reduce();
}

// Constructor with numerator, denominator, and sign
// If denominator is zero, throw an exception
// Assume numerator and denominator are both positive
// Automatically reduce the fraction to lowest terms
Fraction::Fraction(int numerator, int denominator, int sign) {
    if (denominator == 0) {
        throw "Denominator cannot be zero";
    }
    this->numerator = abs(numerator);
    this->denominator = abs(denominator);
    this->sign = sign;
    reduce();
}

Fraction::Fraction(const Fraction& other) {
    numerator = other.numerator;
    denominator = other.denominator;
    sign = other.sign;
    reduce();
}

// Arthematical operators
Fraction Fraction::operator+(const Fraction& other) const {
    // reduce the two fractions to a common denominator
    int new_denominator = lcm(denominator, other.denominator);
    int new_numerator = sign * numerator * (new_denominator / denominator) + other.sign * other.numerator * (new_denominator / other.denominator);
    
    // The constructor will solve the sign and reduce the fraction automatically
    return Fraction(new_numerator, new_denominator);
}

Fraction Fraction::operator-(const Fraction& other) const {
    // reduce the two fractions to a common denominator
    int new_denominator = lcm(denominator, other.denominator);
    int new_numerator = sign * numerator * (new_denominator / denominator) - other.sign * other.numerator * (new_denominator / other.denominator);

    // The constructor will solve the sign and reduce the fraction automatically
    return Fraction(new_numerator, new_denominator);
}

Fraction Fraction::operator*(const Fraction& other) const {
    return Fraction(numerator * other.numerator, denominator * other.denominator, sign * other.sign);
}

Fraction Fraction::operator/(const Fraction& other) const {
    return Fraction(numerator * other.denominator, denominator * other.numerator, sign * other.sign);
}

// Comparison operators
bool Fraction::operator==(const Fraction& other) const {
    // Since the fractions are reduced, we only need to compare the signs, numerator, and denominator
    return sign == other.sign && numerator == other.numerator && denominator == other.denominator;
}

bool Fraction::operator!=(const Fraction& other) const {
    return !(*this == other);
}

bool Fraction::operator<(const Fraction& other) const {
    // To compare two fractions, we first need to compare their signs
    if (sign < other.sign) {
        return true;
    } else if (sign > other.sign) {
        return false;
    } else {  // If their signs are equal, compare their absolute values
        if (sign == 0) return false;  // If both fractions are zero, they are equal

        // reduce the two fractions to a common denominator
        int lcm_val = lcm(denominator, other.denominator);
        int new_numerator1 = numerator * (lcm_val / denominator);
        int new_numerator2 = other.numerator * (lcm_val / other.denominator);
        
        if (sign == 1) {  // If both fractions are positive, the fraction with smaller absolute value is smaller
            return new_numerator1 < new_numerator2;
        } else {  // If both fractions are negative, the fraction with larger absolute value is smaller
            return new_numerator1 > new_numerator2;
        }
    }
}

bool Fraction::operator<=(const Fraction& other) const {
    return *this == other || *this < other;
}

bool Fraction::operator>(const Fraction& other) const {
    return !(*this <= other);
}

bool Fraction::operator>=(const Fraction& other) const {
    return !(*this < other);
}

// @brief Convert Fraction to a double
double Fraction::to_double() const {
    return static_cast<double>(numerator) / denominator * sign;
}

// @brief Convert Fraction to a string (fraction)
std::string Fraction::to_string() const {
    std::string result = "";
    if (sign == 0) {    // If the fraction is zero, we only need to output "0"
        result += "0";
    } else {
        if (sign == -1) {  // If the fraction is negative, output "-" before the numerator
            result += "-";
        }

        // Output the numerator
        result += std::to_string(numerator);

        // If the denominator is 1, the denominator will be omitted
        if (denominator != 1) {
            result += "/" + std::to_string(denominator);
        }
    }
    return result;
}

// @brief Convert a string (fraction) to Fraction
void Fraction::from_string(const std::string& str) {
    std::string buf = str;

    // If a minus sing is found, the fraction is negative
    int minus_pos = buf.find('-');
    if (minus_pos != std::string::npos) {
        sign = -1;
        buf.erase(0, minus_pos + 1);  // Erase the minus sign
    } else {
        sign = 1;
    }

    // Find the slash position
    int slash_pos = buf.find('/');
    if (slash_pos == std::string::npos) {
        // If no slash is found, the fraction is an integer
        // We can set the denominator to 1 and set the numerator to the integer value
        numerator = std::stoi(buf);
        denominator = 1;
    } else {
        // If a slash is found, parse the numerator and denominator respectively
        numerator = std::stoi(buf.substr(0, slash_pos));
        denominator = std::stoi(buf.substr(slash_pos + 1));
    }

    // If the numerator is zero, we need to set the sign to 0
    if (numerator == 0) {
        sign = 0;
    }

    // Reduce the fraction to lowest terms
    reduce();
}

// @brief Convert a string (decimal) to Fraction
void Fraction::from_decimal_string(const std::string& str) {
    std::string buf = str;

    // If a minus sing is found, the fraction is negative
    int minus_pos = buf.find('-');
    if (minus_pos != std::string::npos) {
        sign = -1;
        buf.erase(0, minus_pos + 1);  // Erase the minus sign
    } else {
        sign = 1;
    }

    // Find the dot position
    int dot_pos = buf.find('.');
    if (dot_pos == std::string::npos) {
        // If no dot is found, the fraction is an integer
        // We can set the denominator to 1 and set the numerator to the integer value
        numerator = std::stoi(buf);
        denominator = 1;
    } else {
        // If a dot is found, parse the integer and decimal parts respectively
        int integer_part = (dot_pos == 0) ? 0 : std::stoi(buf.substr(0, dot_pos));
        int decimal_part = std::stoi(buf.substr(dot_pos + 1));

        // We can set Fraction = integer_part * 10^n + decimal_part
        // where n is the number of digits in the decimal part
        int decimal_digit_num = get_digit_num(decimal_part);
        numerator = integer_part * pow(10, decimal_digit_num) + decimal_part;
        denominator = pow(10, decimal_digit_num);
    }

    // If the numerator is zero, we need to set the sign to 0
    if (numerator == 0) {
        sign = 0;
    }

    // Reduce the fraction to lowest terms
    reduce();
}

// Overload the << and >> operators for output and input
std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    os << f.to_string();
    return os;
}

std::istream& operator>>(std::istream& is, Fraction& f) {
    std::string str;
    is >> str;
    
    // Try to parse the string as a fraction first, then as a decimal
    if (str.find('/') != std::string::npos) {
        f.from_string(str);
    } else {
        f.from_decimal_string(str);
    }
    return is;
}

