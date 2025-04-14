/*
 * date.h - Defines the Date class, which is used to represent a date in diary metadata.
 * The Date class provides methods to convert a date string to a Date object, and to compare two dates.
 *
 * Implemented in this file
 */

#pragma once

#include "defs.h"
#include <string>
#include <sstream>

class Date {
    private:
        int year;
        int month;
        int day;

        // The number of days in each month (shared by all instances)
        // Implemented in structures.cpp
        const static int MONTH_DAYS[13];
    
    public:
        // Constructor
        // @brief Construct a Date object with the given year, month, and day.
        Date(int y=0, int m=0, int d=0): year(y), month(m), day(d) {}

        // @brief Construct a Date object from a date string in the format "YYYY-MM-DD".
        Date(const std::string& date_str) {
            std::stringstream ss(date_str);
            std::string data_str;

            // read year
            std::getline(ss, data_str, DATE_DELIMITER);
            year = std::stoi(data_str);
            // read month
            std::getline(ss, data_str, DATE_DELIMITER);
            month = std::stoi(data_str);
            // read day
            std::getline(ss, data_str);
            day = std::stoi(data_str);
        }

        // Comparison operators
        bool operator<(const Date& other) const {
            return year < other.year || 
                   (year == other.year && month < other.month) ||
                   (year == other.year && month == other.month && day < other.day);
        }
        bool operator==(const Date& other) const {
            return year == other.year && month == other.month && day == other.day;
        }
        bool operator!=(const Date& other) const {
            return !(*this == other);
        }
        bool operator<=(const Date& other) const {
            return *this < other || *this == other;
        }
        bool operator>(const Date& other) const {
            return !(*this <= other);
        }
        bool operator>=(const Date& other) const {
            return !(*this < other);
        }

        // Helper methods
        // @brief Check if the year is a leap year.
        bool is_leap_year() const {
            return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
        }

        // @brief Check if the date is valid (i.e. within the range of a month).
        bool is_valid() const {
            return month >= 1 && month <= 12 && day >= 1 && day <= MONTH_DAYS[month] + (month == 2 && is_leap_year());
        }

        // @brief Convert the date to a string in the format "YYYY-MM-DD".
        std::string to_string() const {
            std::string year_str = std::to_string(year);
            std::string month_str = (month < 10 ? "0" : "") + std::to_string(month);
            std::string day_str = (day < 10 ? "0" : "") + std::to_string(day);
            return year_str + DATE_DELIMITER + month_str + DATE_DELIMITER + day_str;
        }
    };
