#include "diary_manager.h"
#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<Date> dates;

    std::string date_str;
    if (argc == 1) {     // If no date is specified in command line, read from stdin.
        Line line;
        // Read lines from stdin until EOF is reached.
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                continue;
            }

            // [Format] ID: DATE (e.g. 1: 2022-01-01)
            std::stringstream ss(line);
            std::string id_str, date_str;
            ss >> id_str >> date_str;

            // Parse date string and check if it is valid.
            Date date = Date(date_str);
            if (date.is_valid()) {
                dates.push_back(date);
            } else {
                std::cout << "Invalid date: " << date_str << std::endl;
            }
        }
    } else {
        for (int i = 1; i < argc; i++) {
            // Parse date string and check if it is valid.
            Date date = Date(argv[i]);
            if (date.is_valid()) {
                dates.push_back(date);
            } else {
                std::cout << "Invalid date: " << argv[i] << std::endl;
            }
        }
    }

    // If no date is specified, return error.
    if (dates.empty()) {
        std::cout << "No date specified." << std::endl;
        return -1;
    }

    std::string filepath = "diary.pdi";
    DiaryManager dmanager(filepath);

    // Get diary for each date and print it.
    for (auto date : dates) {
        // Call get_diary() to get diary for the specified date.
        Diary diary = dmanager.get_diary(date);
        if (!diary.valid) {   // If not found, print error message.
            std::cout << "No diary for " + date.to_string() << std::endl;
            continue;
        }

        // Print diary metadata and content.
        std::cout << "Date: " + date.to_string() << std::endl;
        std::cout << "Title: " << diary.metadata.get_title() << std::endl;
        for (auto line : diary.content) {
            std::cout << line << std::endl;
        }
        std::cout << "---------------------------------------------------" << std::endl;
    }
    
    return 0;
}
