#include "diary_manager.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

// @brief Check if a file exists in the given filepath.
bool file_exists(const std::string& filepath) {
    return std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath);
}

// @brief Skip num_lines lines in the input file.
void skip_line(std::ifstream& input, int num_lines) {
    std::string line;
    for (int i = 0; i < num_lines; i++) {
        std::getline(input, line);
    }
}

// @brief Write num_lines empty lines in the output file.
void empty_line(std::ofstream& output, int num_lines) {
    for (int i = 0; i < num_lines; i++) {
        output << std::endl;
    }
}

// The default file path of the .pdi file
const std::string DEFAULT_FILE_PATH = "diary.pdi";

DiaryManager::DiaryManager(): filepath(DEFAULT_FILE_PATH) {
    initialize();           // Set the DiaryManager to initial empty state
}

DiaryManager::DiaryManager(const std::string& filepath): filepath(filepath) {
    if (!file_exists(filepath)) {
        initialize();       // If the .pdi file not exists, set the DiaryManager to initial empty state
    } else {
        load(filepath);     // If the .pdi file exists, load diaries from the file
    }
}

void DiaryManager::initialize() {
    metadata_list.clear();   // Clear the metadata list
    content_list.clear();    // Clear the content list
    reorganize();            // Reset the header and metadata list
}

void DiaryManager::reorganize() {
    int sum_lines = 0;
    for (int i = 0; i < metadata_list.size(); i++) {
        metadata_list[i].set_start_line(sum_lines);      // Set the start line of each metadata
        sum_lines += metadata_list[i].get_num_lines();
    }

    // Adjust the information in the header
    header.start_line[SECTION_HEADER] = HEADER_BASE;
    header.start_line[SECTION_METADATA] = HEADER_BASE + HEADER_LENGTH;
    header.start_line[SECTION_CONTENT] = HEADER_BASE + HEADER_LENGTH + metadata_list.size();
    header.start_line[END_OF_FILE] = HEADER_BASE + HEADER_LENGTH + metadata_list.size() + sum_lines;
}

bool DiaryManager::load(const std::string& filepath) {
    std::ifstream input(filepath, std::ios::in);
    
    // Load and parse the header
    assert_true_return(header.parse_header(input), "Header parse error");

    // Jump to the start of the metadata section
    int curr_line = HEADER_BASE + HEADER_LENGTH;
    skip_line(input, header.start_line[SECTION_METADATA] - curr_line);
    curr_line = header.start_line[SECTION_METADATA];
    
    // Load the metadata list
    for (int i = 0; i < header.num_diaries; i++) {
        Line line;
        std::getline(input, line);
        if (line.empty()) {   // Skip empty lines
            continue;
        }

        // Parse the metadata line and add it to the metadata list
        // Set the index to specify the order of the diaries in the content list
        metadata_list.push_back(Metadata(line, i));
        curr_line++;
    }

    // Jump to the start of the content section
    skip_line(input, header.start_line[SECTION_CONTENT] - curr_line);
    curr_line = header.start_line[SECTION_CONTENT];

    // Load the content list
    for (int i = 0; i < metadata_list.size(); i++) {
        // Jump to the start of the content of the i-th diary
        int start_line = header.start_line[SECTION_CONTENT] + metadata_list[i].get_start_line();
        skip_line(input, start_line - curr_line);
        curr_line = start_line;

        // Load the content of the i-th diary
        Content content;
        for (int j = 0; j < metadata_list[i].get_num_lines(); j++) {
            Line line;
            std::getline(input, line);
            content.push_back(line);
            curr_line++;
        }
        // Add the loaded content to the content list
        content_list.push_back(content);
    }

    // Sort the metadata list by date
    std::sort(metadata_list.begin(), metadata_list.end());

    // Initialize the deleted flag of the content list to false
    content_list.init_deleted(false);

#ifdef DEBUG
    output_debug_info("Load end");
#endif

    input.close();
    return true;
}

bool DiaryManager::save(const std::string& filepath) {
    // Generate the header parameters and update the metadata list
    reorganize();

#ifdef DEBUG
    output_debug_info("Save start");
#endif

    std::string output_filepath = filepath.empty() ? this->filepath : filepath;
    std::ofstream output(output_filepath, std::ios::out);

    // Save the header
    assert_true_return(header.write_header(output), "Header write error");

    // Save the metadata list
    for (int i = 0; i < metadata_list.size(); i++) {
        output << metadata_list[i].to_string() << std::endl;
    }

    // Save the content list
    for (int i = 0; i < metadata_list.size(); i++) {
        int index = metadata_list[i].get_index();
        for (int j = 0; j < metadata_list[i].get_num_lines(); j++) {
            output << content_list[index][j] << std::endl;
        }
    }

    return true;
}

void DiaryManager::add_diary(const Diary& diary) {
    if (!diary.valid) {
        return;     // If the diary is invalid, do nothing
    }

    // We only consider the Date and Title fields in the input diary
    // THe storage parameters of the diary need to be set in the following
    Metadata metadata = diary.metadata;
    metadata.set_num_lines(diary.content.size());
    metadata.set_index(header.num_diaries);
    
    // Add the diary to the content and metadata list
    content_list.push_back(diary.content);
    metadata_list.push_back(metadata);
    header.num_diaries++;
}

bool DiaryManager::remove_diary(const Date& date) {
    // Binary search the metadata list to find the index of the diary to be removed
    int mid = metadata_list.binary_search(date);
    if (mid == -1) {   // If not found, return false
        std::cerr << "Date not found" << std::endl;
        return false;
    }

    // Delete the metadata of the diary
    content_list.set_deleted(metadata_list[mid].get_index(), true);
    // Fake delete the content of the diary
    metadata_list.erase(metadata_list.begin() + mid);
    header.num_diaries--;
    return true;
}

MetadataRecord DiaryManager::get_metadata_list(Date start_date, Date end_date) {
    // Check the validity of the date range
    if (end_date < start_date) {
        std::cerr << "Invalid date range" << std::endl;
        return MetadataRecord();
    }
    
    // Filter the metadata list by date range
    MetadataRecord result;
    for (int i = 0; i < metadata_list.size(); i++) {
        if (metadata_list[i].get_date() >= start_date && metadata_list[i].get_date() <= end_date) {
            result.push_back(metadata_list[i]);
        }
    }
    return result;
}

Diary DiaryManager::get_diary(const Date& date) {
    // Binary search the metadata list to find the index of the diary to be returned
    int mid = metadata_list.binary_search(date);
    if (mid == -1) {  // If not found, return an invalid diary
        std::cerr << "Date not found" << std::endl;
        return Diary(date, Content(), false);
    }
    
    // Get the index of the diary content in the content list
    int index = metadata_list[mid].get_index();
    // Return the metadata and content of the diary
    return Diary(metadata_list[mid], content_list[index], true);
}

#ifdef DEBUG
void DiaryManager::output_debug_info(std::string message) const {
    std::cout << "**********************************************" << std::endl;
    std::cout << "[Debug] " << message << std::endl;
    std::cout << "Header: " << std::endl;
    std::cout << "file_header_str: " << header.file_header_str << std::endl;
    std::cout << "num_diaries: " << header.num_diaries << std::endl;
    std::cout << "start_line: " << std::endl;
    std::cout << "SECTION_HEADER: " << header.start_line[SECTION_HEADER] << std::endl;
    std::cout << "SECTION_METADATA: " << header.start_line[SECTION_METADATA] << std::endl;
    std::cout << "SECTION_CONTENT: " << header.start_line[SECTION_CONTENT] << std::endl;
    std::cout << "END_OF_FILE: " << header.start_line[END_OF_FILE] << std::endl;

    std::cout << "Metadata list: " << std::endl;
    std::cout << "size: " << metadata_list.size() << std::endl;
    for (int i = 0; i < metadata_list.size(); i++) {
        std::cout << metadata_list[i].to_string() << ", index: " << metadata_list[i].get_index() << std::endl;
    }

    std::cout << "Content list: " << std::endl;
    std::cout << "size: " << content_list.size() << std::endl;
    for (int i = 0; i < metadata_list.size(); i++) {
        int index = metadata_list[i].get_index();
        for (int j = 0; j < metadata_list[i].get_num_lines(); j++) {
            std::cout << content_list[index][j] << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "**********************************************" << std::endl;
}

#endif
