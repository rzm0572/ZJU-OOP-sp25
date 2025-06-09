#pragma once

#include <cstddef>
#include <limits>

template<class _Tp, size_t BlockSize = 4096>
class mAllocator{
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

    union slot {
        value_type data;
        slot* next;
    };

    class Block {
    public:
        Block(Block* _next = nullptr) noexcept
            : next(_next), free_list(nullptr) {
            for (size_type i = SLOTS_PER_BLOCK - 1; i > 0; --i) {
                slots[i].next = free_list;
                free_list = &slots[i];
            }
        }
        ~Block() = default;

        size_type allocate_slot(void *&ptr, size_type n = 1) {
            if (free_list == nullptr) {
                return 0;
            }
            
            ptr = free_list;
            size_type allocated = 0;

            for (allocated = 0; allocated < n; ++allocated) {
                if (free_list == nullptr) {
                    break;
                }
                free_list = free_list->next;
            }
            return allocated;
        }

        void deallocate_slot(void *ptr) {
            if (ptr == nullptr) {
                return;
            }
            ((slot*)ptr)->next = free_list;
            free_list = (slot*)ptr;
        }

    private:
        static constexpr size_type META_SIZE = sizeof(Block*) + sizeof(slot*);
        static constexpr size_type SLOTS_PER_BLOCK = (BlockSize - META_SIZE) / sizeof(slot);
    private:
        Block* next;
        slot* free_list;
        slot slots[SLOTS_PER_BLOCK];
    };

    static Block* avail_blocks;
    static Block* full_blocks;

    Block* new_block() {
        Block* block = new Block(avail_blocks);
        avail_blocks = block;
        return block;
    }

    Block* get_avail_block() {
        if (avail_blocks == nullptr) {
            return new_block();
        }
        return avail_blocks;
    }
};

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator() noexcept {
    avail_blocks = nullptr;
    full_blocks = nullptr;
}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator& mallocator) noexcept {}

template<typename _Tp, size_t BlockSize>
template<class _Up>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator<_Up>& other) noexcept {}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::~mAllocator() {
    while (avail_blocks != nullptr) {
        Block* block = avail_blocks;
        avail_blocks = block->next;
        delete block;
    }
    while (full_blocks != nullptr) {
        Block* block = full_blocks;
        full_blocks = block->next;
        delete block;
    }
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::pointer
mAllocator<_Tp, BlockSize>::address(reference x) const noexcept {
    return &x;
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::const_pointer
mAllocator<_Tp, BlockSize>::address(const_reference x) const noexcept {
    return &x;
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::pointer
mAllocator<_Tp, BlockSize>::allocate(size_type n, const _Not_user_specialized* hint) {
    
}

template<typename _Tp, size_t BlockSize>
void mAllocator<_Tp, BlockSize>::deallocate(pointer p, size_type n) {

}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::size_type
mAllocator<_Tp, BlockSize>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template<typename _Tp, size_t BlockSize>
template<class _Up, class... Args>
void mAllocator<_Tp, BlockSize>::construct(_Up* p, Args&&... args) {

}

template<typename _Tp, size_t BlockSize>
template<class _Up>
void mAllocator<_Tp, BlockSize>::destroy(_Up* p) {

}

