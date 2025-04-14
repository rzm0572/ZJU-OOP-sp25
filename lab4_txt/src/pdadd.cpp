#include "diary_manager.h"
#include "interaction.h"
#include <iostream>

int main() {
    std::string filepath = "diary.pdi";
    DiaryManager dmanager(filepath);

    // Check if the input is from a terminal or a pipe
    // source = 1 if input is from a terminal, 0 if input is from a pipe
    int source = isatty(0);

    // Get the date and the title of the diary from stdin
    std::string date_str, title;
    PROMPT(source, "Enter the date (YYYY-MM-DD): ");
    std::cin >> date_str;
    std::cin.get();
    PROMPT(source, "Enter the diary title: ");
    std::getline(std::cin, title);


    // Parse the date string and check if it is valid
    Date date = Date(date_str);
    if (!date.is_valid()) {
        std::cout << "Invalid date format." << std::endl;
        return -1;
    }

    // Create the metadata structure based on the date and title
    Metadata metadata(date, title);

    // Get the content of the diary from stdin
    Content content;
    PROMPT(source, "Enter the diary content: " << std::endl << "> ");
    Line line;
    // Read the content line by line
    while (std::getline(std::cin, line)) {
        if (line == ".") {
            break;
        }
        content.push_back(line);
        PROMPT(source, "> ");
    }

    // Call the add_diary method of the DiaryManager to add the diary to the file
    dmanager.add_diary(Diary(metadata, content));
    // Save the update to the file
    dmanager.save();
    return 0;

}
