/*
 * defs.h - Contains common definitions used in the personal diary lib.
 */

#pragma once

#include <string>

// helper macros

// Return false and print message if condition is false
#define assert_true_return(condition, message) \
    if (!(condition)) {                        \
        std::cerr << message << std::endl;     \
        return false;                          \
    }

// Return false and print message if condition is true
#define assert_false_return(condition, message) \
    if (condition) {                            \
        std::cerr << message << std::endl;      \
        return false;                           \
    }

typedef std::string Line;       // a line of text in diary content
typedef std::string Title;      // the title of a diary

// section types
// Section is used to manage the storage of diary in .pdi file
enum Section {
    SECTION_HEADER = 0,        // header section
    SECTION_METADATA = 1,      // metadata section
    SECTION_CONTENT = 2,       // content section
    END_OF_FILE = 3            // end of file
};

// Common constants
const int NUM_SECTIONS = 4;             // number of sections in .pdi file
const int HEADER_BASE = 0;              // base line number of header section
const int HEADER_LENGTH = 2;            // number of lines in header section
const char METADATA_DELIMITER = ';';    // delimiter between metadata values
const char DATE_DELIMITER = '-';        // delimiter in date values (e.g '-' in 2021-01-01)
const std::string FILE_HEADER_STR = "Personal Diary Data File";  // header string in .pdi file
