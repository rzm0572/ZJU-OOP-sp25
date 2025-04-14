/*
 * header.h - Defines the Header struct and related functions.
 *
 * Implemented in structures.cpp
 */

#pragma once

#include "defs.h"

// Header struct is used to store the basic information extracted from .pdi file
struct Header {
    std::string file_header_str;    // The first line of the file, which is used to identify the file type
    int num_diaries;                // The number of diaries in the file
    int start_line[NUM_SECTIONS];   // The line number where each section starts

    // @brief Default constructor, which initializes the header as the header of a blank diary file
    Header();
    
    // @brief Constructor that takes in the file header string, number of diaries, and start line of each section
    Header(const std::string& file_header_str, int num_diaries, int start_line_metadata, int start_line_content, int end_line);
    
    // @brief Parses the header from the input stream and stores the information in the Header struct
    // @return true if the header is successfully parsed, false otherwise
    bool parse_header(std::ifstream& input);

    // @brief Writes the header to the output stream
    // @return true if the header is successfully written, false otherwise
    bool write_header(std::ofstream& output);
};
