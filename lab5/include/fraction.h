#ifndef __FRACTION_H__
#define __FRACTION_H__

#include <iostream>
#include <string>

class Fraction {
private:
    int numerator;
    int denominator;
    int sign;   // the sign of the fraction, 1 for positive, -1 for negative, 0 for zero

    // @brief Reduce the fraction to lowest terms
    void reduce();

    // friend functions for input/output stream
    friend std::istream& operator>>(std::istream& is, Fraction& f);
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);

public:
    Fraction();
    Fraction(int numerator, int denominator);
    Fraction(int numerator, int denominator, int sign);
    Fraction(const Fraction& other);

    // overload arthematical operators
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;

    // overload comparison operators
    bool operator==(const Fraction& other) const;
    bool operator!=(const Fraction& other) const;
    bool operator<(const Fraction& other) const;
    bool operator<=(const Fraction& other) const;
    bool operator>(const Fraction& other) const;
    bool operator>=(const Fraction& other) const;

    // convert Fraction to other types
    double to_double() const;
    std::string to_string() const;

    // convert Fraction from other types
    void from_string(const std::string& str);
    void from_decimal_string(const std::string& str);
};

#endif
