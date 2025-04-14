#pragma once

#include "metadata.h"
#include <string>

class Diary {
private:
    MetadataBasis metadata;
    std::string content;
public:
    Diary(): metadata(), content() {}
    Diary(const MetadataBasis& metadata, const std::string& content): metadata(metadata), content(content) {}

    const MetadataBasis& get_metadata() const {
        return metadata;
    }
};
