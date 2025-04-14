/*
 * metadata.h - Defines the MetadataBasis, Metadata and MetadataList classes, and MetadataRecord type.
 *
 * Implemented in structures.cpp
 */

#pragma once

#include "date.h"

// MetadataBasis class represents the basic information of a diary, including date and title.
class MetadataBasis {
protected:
    Date date;      // the date of the diary
    Title title;    // the title of the diary
public:
    MetadataBasis(const Date& date = Date(), const Title& title = Title()): date(date), title(title) {}

    // @brief Returns a string representation of the metadata in the format of "YYYY-MM-DD Title"
    std::string to_string() const {
        return date.to_string() + " " + title;
    }

    // @brief Returns the date of the metadata
    Title get_title() const {
        return title;
    }
};

// Metadata class inherites from MetadataBasis and adds storage information of the diary, including the start line and the number of lines of the diary content in the file, and the index of the diary content stored in ContentList.
class Metadata: public MetadataBasis {
private:
    int start;     // The start line of the diary content in .pdi file
    int length;    // The number of lines of the diary content in .pdi file
    int index;     // The index of the diary content in ContentList
public:
    // @brief Constructor of Metadata class
    Metadata(const Date& date = Date(), const Title& title = Title(), int start = 0, int length = 0, int index = 0): MetadataBasis(date, title), start(start), length(length), index(index) {}

    // @brief Constructor of Metadata class from a line in the metadata section of the .pdi file
    // @param line: A line in the metadata section of the .pdi file, in the format of "YYYY-MM-DD Title start length"
    // @param index: the index of the corresponding diary content in ContentList
    Metadata(const Line& line, int index = 0);

    bool operator<(const Metadata& other) const {
        return date < other.date;
    }

    int get_index() const {
        return index;
    }

    int get_start_line() const {
        return start;
    }

    int get_num_lines() const {
        return length;
    }

    Date get_date() const {
        return date;
    }

    void set_index(int index) {
        this->index = index;
    }

    void set_start_line(int start_line) {
        start = start_line;
    }

    void set_num_lines(int num_lines) {
        length = num_lines;
    }

    // @brief Returns a string representation of the metadata in the format of "YYYY-MM-DD Title start length"
    Line to_string() const {
        return date.to_string() + METADATA_DELIMITER + title + METADATA_DELIMITER + std::to_string(start) + METADATA_DELIMITER + std::to_string(length);
    }
};

// MetadataRecord is a vector of MetadataBasis objects
// This class is one of the exchanged data types between the diary manager and the user program, used by the function get_metadata_list() in diary_manager.cpp.
typedef std::vector<MetadataBasis> MetadataRecord;

// MetadataList is a vector of Metadata objects
// This class is used to store the metadata of all diaries in the diary manager.
// It provides binary search function to find the metadata of a diary by date.
class MetadataList: public std::vector<Metadata> {
public:
    MetadataList() {}

    // @brief Searches for the metadata of a diary by date using binary search
    int binary_search(const Date& date) const;
};
