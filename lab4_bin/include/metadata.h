#pragma once

#include "page_table.h"
#include <cstdint>
#include <string>
#include <vector>

class MetadataBasis {
public:
    uint64_t diary_id;
    uint64_t date;
    std::string title;

    MetadataBasis(uint64_t diary_id = 0, uint64_t date = 0, const std::string& title = ""): diary_id(diary_id), date(date), title(title) {}
};

class Metadata: public MetadataBasis {
public:
    uint64_t next_metadata_page;
    PageIndexList page_index_list;

    Metadata(): MetadataBasis(), next_metadata_page(INVALID_PAGE) {
        page_index_list.clear();
    }

    uint64_t get_title_size() const {
        return title.size();
    }

    uint64_t get_page_index_list_size() const {
        return page_index_list.size();
    }
};

class MetadataList {
private:
    std::vector<Metadata> metadata_list;
public:
    MetadataList();

    void add_metadata(const Metadata& metadata);
};
