/* 
 * content.h - Defines the Content type and ContentList class
 * Content is a vector of lines.
 * ContentList inherites the vector of Content and adds a deleted flag for each content.
 *
 * Implemented in this file
 */
#pragma once

#include "defs.h"
#include <vector>

// Content is a vector of lines
typedef std::vector<Line> Content;

// ContentList inherites the vector of Content and adds a deleted flag for each content.
class ContentList: public std::vector<Content> {
private:
    std::vector<bool> deleted;  // deleted flag for each content
public:
    ContentList(): std::vector<Content>(), deleted() {}

    // @brief get the deleted flag of the content at index
    bool is_deleted(int index) const {
        return deleted[index];
    }

    // @brief set the deleted flag of the content at index to value
    void set_deleted(int index, bool value) {
        this->deleted[index] = value;
    }

    // @brief initialize the deleted flag of all contents to value
    void init_deleted(bool value) {
        deleted.assign(this->size(), value);
    }
};
