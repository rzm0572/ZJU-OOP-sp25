#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <new>
#include <iostream>
#include <cstdlib>

#ifndef DEFAULT_ALLOC_SLOT
    #define DEFAULT_ALLOC_SLOT 20
#endif

class MallocAllocator {
public:
    static void* allocate(size_t size) {
        void* result = malloc(size);
        return result;
    }
    static void deallocate(void* ptr) {
        free(ptr);
    }
};

template<class _Tp, size_t BlockSize = 4096>
class mAllocator {
public:
    using _Not_user_specialized = void;
    using value_type = _Tp;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template <class _Up> struct rebind {
        using other = mAllocator<_Up, BlockSize>;
    };

    mAllocator() noexcept;
    mAllocator(const mAllocator& mallocator) noexcept;
    template<class _Up> mAllocator(const mAllocator<_Up>& other) noexcept;
    ~mAllocator();

    pointer address(reference x) const noexcept;
    const_pointer address(const_reference x) const noexcept;
    pointer allocate(size_type n, const _Not_user_specialized* hint = 0);
    void deallocate(pointer p, size_type n);
    size_type max_size() const noexcept;

    template<class _Up, class... Args>
    void construct(_Up* p, Args&&... args);

    template<class _Up>
    void destroy(_Up* p);

private:

    constexpr static size_t BLOCK_SIZE = BlockSize;
    constexpr static int SLOT_SIZE_NUM = 16;
    constexpr static int SLOT_SIZE_AVAILABLE[SLOT_SIZE_NUM] = {
        8, 16, 24, 32, 40, 48, 56, 64,
        72, 80, 88, 96, 104, 112, 120, 128
    };

    union slot_t {
        slot_t* next;
        _Tp data[0];
    };

    int get_slot_index(size_t size) {
        return size > 128 ? -1 : (size - 1) >> 3;
    }

    static slot_t* free_slot_list[SLOT_SIZE_NUM];
    static uint8_t* memory_pool_start;
    static uint8_t* memory_pool_end;

    void* extend_free_slot_list(size_type size);

    void* alloc_slot_mempool(size_type size, int count);

    void inflate_mempool(size_type size);

};

template<class _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::slot_t* mAllocator<_Tp, BlockSize>::free_slot_list[SLOT_SIZE_NUM] = {nullptr};

template<class _Tp, size_t BlockSize>
uint8_t* mAllocator<_Tp, BlockSize>::memory_pool_start = nullptr;

template<class _Tp, size_t BlockSize>
uint8_t* mAllocator<_Tp, BlockSize>::memory_pool_end = nullptr;

template<class _Tp, size_t BlockSize>
void* mAllocator<_Tp, BlockSize>::extend_free_slot_list(size_type size) {
    int count = DEFAULT_ALLOC_SLOT;
    int slot_index = get_slot_index(size);
    int alloc_size = SLOT_SIZE_AVAILABLE[slot_index];
    if (slot_index == -1) {
        return nullptr;
    }

    slot_t* slot = reinterpret_cast<slot_t*>(alloc_slot_mempool(alloc_size, count));
    for (int i = 0; i < count; i++) {
        slot->next = free_slot_list[slot_index];
        free_slot_list[slot_index] = slot;
        slot = reinterpret_cast<slot_t*>(reinterpret_cast<uint8_t*>(slot) + alloc_size);
    }
    return free_slot_list[slot_index];
}

template<class _Tp, size_t BlockSize>
void* mAllocator<_Tp, BlockSize>::alloc_slot_mempool(size_type size, int count) {
    size_type alloc_size = size * count;
    if (memory_pool_start + alloc_size > memory_pool_end) {
        inflate_mempool(alloc_size);
    }

    slot_t* slot = reinterpret_cast<slot_t*>(memory_pool_start);
    memory_pool_start = reinterpret_cast<uint8_t*>(memory_pool_start) + alloc_size;
    return slot;
}

template<class _Tp, size_t BlockSize>
void mAllocator<_Tp, BlockSize>::inflate_mempool(size_type size) {
    size_type alloc_size = (size + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE;

    memory_pool_start = static_cast<uint8_t*>(malloc(alloc_size));
    if (!memory_pool_start) {
        memory_pool_end = nullptr;
        throw std::bad_alloc();
    }

    memory_pool_end = memory_pool_start + alloc_size;    
}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator() noexcept {
    
}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator& mallocator) noexcept {}

template<typename _Tp, size_t BlockSize>
template<class _Up>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator<_Up>& other) noexcept {}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::~mAllocator() {}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::pointer mAllocator<_Tp, BlockSize>::address(reference x) const noexcept {
    return &x;
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::const_pointer mAllocator<_Tp, BlockSize>::address(const_reference x) const noexcept {
    return &x;
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::pointer mAllocator<_Tp, BlockSize>::allocate(size_type n, const _Not_user_specialized* hint) {
    int alloc_size = n * sizeof(value_type);
    int index = get_slot_index(alloc_size);
    if (index == -1) {
        return reinterpret_cast<pointer>(MallocAllocator::allocate(alloc_size));
    } else {
        if (free_slot_list[index] == nullptr) {
            extend_free_slot_list(alloc_size);
        }
        slot_t* slot = free_slot_list[index];
        free_slot_list[index] = slot->next;
        // std::cout << "allocate slot " << slot << " from slot list " << index << std::endl;
        return slot->data;
    }
}

template<typename _Tp, size_t BlockSize>
void mAllocator<_Tp, BlockSize>::deallocate(pointer p, size_type n) {
    if (p == nullptr) {
        throw std::bad_alloc();
    }

    int free_size = n * sizeof(value_type);
    int index = get_slot_index(free_size);
    if (index == -1) {
        MallocAllocator::deallocate(static_cast<void*>(p));
    } else {
        slot_t* slot = reinterpret_cast<slot_t*>(p);
        slot->next = free_slot_list[index];
        free_slot_list[index] = slot;
    }
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::size_type mAllocator<_Tp, BlockSize>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template<typename _Tp, size_t BlockSize>
template<class _Up, class... Args>
void mAllocator<_Tp, BlockSize>::construct(_Up* p, Args&&... args) {
    ::new (static_cast<void*>(p)) _Up(std::forward<Args>(args)...);
}

template<typename _Tp, size_t BlockSize>
template<class _Up>
void mAllocator<_Tp, BlockSize>::destroy(_Up* p) {
    if (p != nullptr) {
        p->~_Up();
    }
}