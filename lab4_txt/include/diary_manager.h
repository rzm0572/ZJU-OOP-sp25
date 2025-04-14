/*
 * diary_manager.h - Defines the DiaryManager classes.
 * DiaryManager is the main class in this lib.
 *
 * Implemented in diary_manager.cpp.
 */

#pragma once

#include "header.h"
#include "date.h"
#include "metadata.h"
#include "content.h"
#include <string>

// Diary is a struct that represents a diary entry.
struct Diary {
    Metadata metadata;
    Content content;
    bool valid;          // Marks whether the diary is valid or not

    Diary(const Metadata& metadata = Metadata(), const Content& content = Content(), bool valid = true): metadata(metadata), content(content), valid(valid) {}
};

// DiaryManager is the main class in this lib.
// It manages the diary entries and provides APIs to manipulate them.
// It also provides APIs to load and save the diary entries to/from a file.
class DiaryManager {
private:
    std::string filepath;
    Header header;                // Header of the diary file.
    MetadataList metadata_list;   // List of metadata of the diaries.
    ContentList content_list;     // List of content of the diaries.

    // @brief Initialize DiaryManager to represent an empty list of diaries.
    void initialize();

    // @brief Reorganize the header and metadata_list to match the content_list
    void reorganize();

public:
    // @brief Construct an empty DiaryManager.
    DiaryManager();
    // @brief Construct a DiaryManager from the .pdi file at the given filepath.
    DiaryManager(const std::string& filepath);
    
    // IO functions
    // @brief Load the diary entries from the file.
    bool load(const std::string& filepath);
    // @brief Save the diary entries to the file.
    bool save(const std::string& filepath = "");

    // Manipulation functions
    // @brief Add a new diary entry to the list.
    void add_diary(const Diary& diary);
    // @brief Remove a diary entry by date from the list.
    bool remove_diary(const Date& date);
    // @brief Get the diary metadata and content by date.
    Diary get_diary(const Date& date);
    // @brief Get the metadata list of diaries between start_date and end_date.
    MetadataRecord get_metadata_list(Date start_date, Date end_date);

#ifdef DEBUG
    // Debug functions
    // @brief Output debug information to the console.
    void output_debug_info(std::string message) const;
#endif
};
