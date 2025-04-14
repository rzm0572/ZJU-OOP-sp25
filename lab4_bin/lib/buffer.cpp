#include "Buffer.h"
#include <cstdint>

void Buffer::resize_buffer_dirty() {
    dirty.resize((this->size() + PAGE_SIZE - 1) / PAGE_SIZE, false);
}

void Buffer::set_buffer_dirty(uint64_t page_index, bool dirty_flag) {
    if (page_index < dirty.size()) {
        dirty[page_index] = dirty_flag;
    }
}

bool Buffer::get_buffer_dirty(uint64_t page_index) const {
    if (page_index < dirty.size()) {
        return dirty[page_index];
    }
    return false;
}

uint64_t Buffer::get_buffer_dirty_size() const {
    return dirty.size();
}

template<typename T>
T Buffer::extract_data(uint64_t& pos) {
    uint64_t type_size = sizeof(T);
    if (pos + type_size > this->size()) {
        throw std::out_of_range("Out of range when extracting data from buffer");
    }

    T data;
    std::copy(this->begin() + pos, this->begin() + pos + type_size, &data);
    pos += type_size;
    return data;
}

template<typename T>
T Buffer::extract_data(Buffer::iterator& iter) {
    uint64_t type_size = sizeof(T);
    if (iter + type_size > this->end()) {
        throw std::out_of_range("Out of range when extracting data from buffer");
    }

    T data;
    std::copy(iter, iter + type_size, &data);
    iter += type_size;
    return data;
}

template<typename T>
void Buffer::update_data(uint64_t& pos, const T& data) {
    uint64_t type_size = sizeof(T);
    if (pos + type_size > this->size()) {
        throw std::out_of_range("Out of range when updating data in buffer");
    }

    T* ptr = reinterpret_cast<T*>(this->data() + pos);
    *ptr = data;
    pos += type_size;
}

template<typename T>
void Buffer::update_data(Buffer::iterator& iter, const T& data) {
    uint64_t type_size = sizeof(T);
    if (iter + type_size > this->end()) {
        throw std::out_of_range("Out of range when updating data in buffer");
    }

    T* ptr = reinterpret_cast<T*>(iter.base());
    *ptr = data;
    iter += type_size;
}
