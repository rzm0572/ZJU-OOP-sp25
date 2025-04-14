#include "page_table.h"
#include "marco.h"
#include <cstdint>
#include <iostream>

PageTable::PageTable() {
    page_entities.clear();
    free_pages.clear();
}

void PageTable::create_initial_page_table() {
    page_entities.push_back(PageTableEntity(PAGE_HEADER));
    page_entities.push_back(PageTableEntity(PAGE_PAGE_TABLE));
}

bool PageTable::load_page_entity(PageTableEntity& page_entity) {
    page_entities.push_back(page_entity);
    return true;
}

bool PageTable::load_page_entity(PageTableEntity& page_entity, uint64_t page_index) {
    if (page_index >= page_entities.size()) {
        return false;
    }
    page_entities[page_index] = page_entity;
    return true;
}

bool PageTable::initialize_free_pages() {
    for (uint64_t i = 0; i < page_entities.size(); i++) {
        if (page_entities[i].page_type == PAGE_FREE) {
            free_pages.push_back(i);
        }
    }
    return true;
}

uint64_t PageTable::allocate_page(PageType page_type) {
    // assert_false_return(free_pages.empty(), "No free pages available");
    if (free_pages.empty()) {
        std::cout << "No free pages available" << std::endl;
        return INVALID_PAGE;
    }
    if (page_type == PAGE_FREE) {
        return INVALID_PAGE;
    }

    uint64_t page_index = free_pages.front();
    free_pages.pop_front();
    page_entities[page_index].page_type = page_type;
    return page_index;
}

bool PageTable::free_page(uint64_t page_index) {
    assert_false_return(page_index >= page_entities.size(), "Invalid page index");
    assert_false_return(is_free_page(page_index), "This page is already free");
    page_entities[page_index].page_type = PAGE_FREE;
    free_pages.push_front(page_index);
    return true;
}

bool PageTable::no_free_pages() const {
    return free_pages.empty();
}

const PageEntityList& PageTable::get_page_entities() const {
    return page_entities;
}

bool PageTable::is_free_page(uint64_t page_index) const {
    if (page_index >= page_entities.size()) {
        return false;
    }
    return page_entities[page_index].page_type == PAGE_FREE;
}
