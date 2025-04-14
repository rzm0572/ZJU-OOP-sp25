#pragma once

#include <cstdint>
#include <list>

enum PageType {
    PAGE_FREE = 0,
    PAGE_HEADER = 1,
    PAGE_PAGE_TABLE = 2,
    PAGE_DATA = 3
};

const uint64_t PAGESIZE = 1024;
const uint64_t INVALID_PAGE = 0xffffffffffffffff;

typedef std::vector<uint64_t> PageIndexList;

struct PageTableEntity {
    PageType page_type;

    PageTableEntity(PageType page_type = PAGE_FREE): page_type(page_type) {}

    PageTableEntity& operator=(const PageTableEntity& other) {
        page_type = other.page_type;
        return *this;
    }
};

typedef std::list<uint64_t> FreePageList;
typedef std::vector<PageTableEntity> PageEntityList;

class PageTable {
private:
    PageEntityList page_entities;
    FreePageList free_pages;

public:
    PageTable();

    void create_initial_page_table();

    // uint64_t get_num_pages() const;

    bool load_page_entity(PageTableEntity& page_entity);

    bool load_page_entity(PageTableEntity& page_entity, uint64_t page_index);

    bool initialize_free_pages();

    uint64_t allocate_page(PageType page_type);

    bool free_page(uint64_t page_index);

    bool no_free_pages() const;

    const PageEntityList& get_page_entities() const;

    bool is_free_page(uint64_t page_index) const;
};
