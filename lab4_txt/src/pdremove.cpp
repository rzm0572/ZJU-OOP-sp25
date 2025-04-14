#include "diary_manager.h"
#include "interaction.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string date_str;

    int source = isatty(0);

    if (argc == 1) {    // If no argument is provided, get date from user input.
        PROMPT(source, "Enter the date (YYYY-MM-DD) to remove: ");
        std::cin >> date_str;
    } else {            // If argument is provided, use it as date.
        date_str = std::string(argv[1]);
    }

    std::string filepath = "diary.pdi";
    DiaryManager dmanager(filepath);

    // Parse the date string and check if it is valid.
    Date date = Date(date_str);
    if (!date.is_valid()) {
        std::cout << "Invalid date format." << std::endl;
        return -1;
    }
    
    // Call the remove_diary function of DiaryManager to remove the diary.
    int ret = dmanager.remove_diary(date);

    // Save the update to the file.
    dmanager.save();

    // If the remove operation is successful, return 0. Otherwise, return -1.
    return ret ? 0 : -1;
}
