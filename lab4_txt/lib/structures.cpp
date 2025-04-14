#include "defs.h"
#include "header.h"
#include "metadata.h"
#include <iostream>
#include <fstream>

const int Date::MONTH_DAYS[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Header::Header() {
    file_header_str = FILE_HEADER_STR;                            // set the file header string
    num_diaries = 0;

    // Create the basic structure of an empty .pdi file
    start_line[SECTION_HEADER] = HEADER_BASE;
    start_line[SECTION_METADATA] = HEADER_BASE + HEADER_LENGTH;
    start_line[SECTION_CONTENT] = HEADER_BASE + HEADER_LENGTH;
    start_line[END_OF_FILE] = HEADER_BASE + HEADER_LENGTH;
}

Header::Header(const std::string& file_header_str, int num_diaries, int start_line_metadata, int start_line_content, int end_line): file_header_str(file_header_str), num_diaries(num_diaries) {
    start_line[SECTION_HEADER] = HEADER_BASE;
    start_line[SECTION_METADATA] = start_line_metadata;
    start_line[SECTION_CONTENT] = start_line_content;
    start_line[END_OF_FILE] = end_line;
}

bool Header::parse_header(std::ifstream& input) {
    // If the ifstream is not open, return false
    assert_true_return(input.is_open(), "Input file is not open");
    
    // Read the first line of the file, which should be the file header string
    std::getline(input, file_header_str);
    // If the file header string is not correct, return false
    assert_true_return(file_header_str == FILE_HEADER_STR, "Not a valid personal diary data file")

    try {
        // Get the basic structure of the file from the header section
        input >> num_diaries >> start_line[SECTION_METADATA] >> start_line[SECTION_CONTENT] >> start_line[END_OF_FILE];
        input.get();    // Handle the \n character at the end of the line
    } catch (...) {
        std::cerr << "Header format error" << std::endl;
        return false;
    }
    start_line[SECTION_HEADER] = HEADER_BASE;

    return true;
}

bool Header::write_header(std::ofstream& output) {
    // If the ofstream is not open, return false
    assert_true_return(output.is_open(), "Output file is not open");

    // Output the file header string and the basic structure of the file to the header section
    output << file_header_str << std::endl;
    output << num_diaries << " " << start_line[SECTION_METADATA] << " " << start_line[SECTION_CONTENT] << " " << start_line[END_OF_FILE] << std::endl;
    return true;
}

Metadata::Metadata(const Line& line, int index) {
    std::stringstream ss(line);

    // Set the date
    std::string data_str;
    std::getline(ss, data_str, METADATA_DELIMITER);
    date = Date(data_str);

    // Set the title
    std::getline(ss, data_str, METADATA_DELIMITER);
    title = data_str;

    // Set the start line of the diary content
    std::getline(ss, data_str, METADATA_DELIMITER);
    start = std::stoi(data_str);
    
    // Set the the number of lines of the diary content
    std::getline(ss, data_str);
    length = std::stoi(data_str);

    // Set the index of the diary
    this->index = index;
}

int MetadataList::binary_search(const Date& date) const {
    // Binary search for the metadata with the given date
    int l = 0, r = this->size() - 1, mid;
    Date mid_date;
    while (l <= r) {
        mid = (l + r) / 2;
        mid_date = this->at(mid).get_date();
        if (mid_date == date) {
            return mid;                  // If the date is found, return the index of the metadata
        } else if (mid_date < date) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return -1;                           // If the date is not found, return -1
}
