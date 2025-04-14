#pragma once

#include <vector>

const uint64_t PAGE_SIZE = 1024;

class Buffer : public std::vector<unsigned char> {
private:
    std::vector<bool> dirty;
public:
    Buffer(): std::vector<unsigned char>(), dirty(0) {}
    Buffer(uint64_t size): std::vector<unsigned char>(size), dirty((size + PAGE_SIZE - 1) / PAGE_SIZE, false) {}

    void resize_buffer_dirty();
    
    void set_buffer_dirty(uint64_t page_index, bool dirty_flag);

    bool get_buffer_dirty(uint64_t page_index) const;

    uint64_t get_buffer_dirty_size() const;

    template<typename T>
    T extract_data(uint64_t& pos);

    template<typename T>
    T extract_data(Buffer::iterator& iter);

    template<typename T>
    void update_data(uint64_t& pos, const T& data);

    template<typename T>
    void update_data(Buffer::iterator& iter, const T& data);
};
