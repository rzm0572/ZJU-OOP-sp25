#include "diary_manager.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {    // Check the number of arguments
        std::cout << "Usage: pdlist <start_date> <end_date>" << std::endl;
        return -1;
    }
    std::string filepath = "diary.pdi";

    // Create a DiaryManager object with the default filepath
    DiaryManager dmanager(filepath);

    // Parse the start_date and end_date from the command line arguments
    Date start_date(argv[1]);
    Date end_date(argv[2]);

    // Check the validity of start_date and end_date
    if (!start_date.is_valid() || !end_date.is_valid()) {
        std::cout << "Invalid date." << std::endl;
        return -1;
    }

    // Call the get_metadata_list function to get the metadata list of diaries between start_date and end_date
    MetadataRecord metadata_record = dmanager.get_metadata_list(start_date, end_date);

    // Print the metadata list
    for (int i = 0; i < metadata_record.size(); i++) {
        std::cout << i << ": " << metadata_record[i].to_string() << std::endl;
    }

    return 0;
}
