#pragma once

#include <cstddef>
#include <limits>
#include <new>
#include <iostream>

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

    template<size_type SlotSize>
    union Slot {
        uint8_t data[SlotSize];
        Slot<SlotSize>* next;
    };

    // 8,16,32,64,128,256,512,1016,2032,4064

    template<size_type SlotSize>
    class Block {
        friend class mAllocator;
    private:
        using Slot = Slot<SlotSize>;

    public:
        Block(Block* _prev = nullptr, Block* _next = nullptr) noexcept
            : prev(_prev), next(_next), free_list(nullptr) {

            slots_used = 0;
            Slot* slots = reinterpret_cast<Slot*>(data);
            for (int64_t i = SLOTS_PER_BLOCK - 1; i >= 0; --i) {
                slots[i].next = free_list;
                free_list = &slots[i];
            }
        }
        ~Block() {
            prev = nullptr;
            next = nullptr;
            free_list = nullptr;
        }

        Slot* allocate_slot() {
            if (is_full()) {
                return nullptr;
            }
            
            Slot* slot = free_list;
            free_list = free_list->next;
            slots_used++;
            return slot;
        }

        void deallocate_slot(pointer ptr) {
            if (ptr == nullptr) {
                return;
            }
            if (is_empty()) {
                return;
            }

            Slot* slot = reinterpret_cast<Slot*>(ptr);
            slot->next = free_list;
            free_list = slot;
            slots_used--;
        }

        bool is_full() const {
            return slots_used == SLOTS_PER_BLOCK;
        }

        bool is_empty() const {
            return slots_used == 0;
        }

        bool is_avail() const {
            return slots_used < BLOCK_AVAILABLE_THRESHOLD;
        }

    private:
        static constexpr size_type META_SIZE = sizeof(Block*) * 2 + sizeof(Slot*) + sizeof(size_type);
        static constexpr size_type SLOTS_PER_BLOCK = (BlockSize - META_SIZE) / sizeof(Slot);
        static constexpr size_type OBJ_PER_SLOT = sizeof(Slot) / sizeof(value_type);
        static constexpr size_type BLOCK_AVAILABLE_THRESHOLD = (size_type)(SLOTS_PER_BLOCK * 1);

    private:
        Block* prev;
        Block* next;
        Slot* free_list;
        int64_t slots_used;
        uint8_t data[BlockSize - META_SIZE];
    };

    template<size_type SlotSize>
    inline static Block<SlotSize>* avail_blocks;

    template<size_type SlotSize>
    inline static Block<SlotSize>* avail_blocks_tail;

    template<size_type SlotSize>
    inline static Block<SlotSize>* full_blocks;

    template<size_type SlotSize>
    Block<SlotSize>* new_block() {
        Block<SlotSize>* block = new (std::align_val_t(BlockSize)) Block<SlotSize>(nullptr, avail_blocks<SlotSize>);
        if (avail_blocks<SlotSize> != nullptr) {
            (avail_blocks<SlotSize>)->prev = block;
        }
        if (avail_blocks_tail<SlotSize> == nullptr) {
            avail_blocks_tail<SlotSize> = block;
        }
        avail_blocks<SlotSize> = block;
        return block;
    }

    template<size_type SlotSize>
    Block<SlotSize>* get_avail_block() {
        if (avail_blocks<SlotSize> == nullptr) {
            return new_block<SlotSize>();
        }
        return avail_blocks<SlotSize>;
    }

    template<size_type SlotSize>
    Slot<SlotSize>* allocate_slot() {
        Block<SlotSize>* block = get_avail_block<SlotSize>();
        Slot<SlotSize>* slot = block->allocate_slot();
        if (slot == nullptr) {
            return nullptr;
        }
        if (block->is_full()) {
            avail_blocks<SlotSize> = block->next;
            if (block->next != nullptr) {
                block->next->prev = nullptr;
            } else {
                avail_blocks_tail<SlotSize> = nullptr;
            }
            if (full_blocks<SlotSize> != nullptr) {
                full_blocks<SlotSize>->prev = block;
            }
            block->prev = nullptr;
            block->next = full_blocks<SlotSize>;
            full_blocks<SlotSize> = block;
            // std::cout << "Move block " << block << " to full_blocks (slots_used: " << block->slots_used << ")" << std::endl;
        }
        return slot;
    }

    template<size_type SlotSize>
    void deallocate_slot(pointer ptr) {
        if (ptr == nullptr) {
            return;
        }

        Block<SlotSize>* block = reinterpret_cast<Block<SlotSize>*>(reinterpret_cast<uint8_t*>(reinterpret_cast<unsigned long>(ptr) & ~(BlockSize - 1)));
        if (block->is_empty()) {
            return;
        }

        block->deallocate_slot(ptr);

        if (block->is_avail()) {
            if (block->prev != nullptr) {
                block->prev->next = block->next;
            } else {
                full_blocks<SlotSize> = block->next;
            }

            if (block->next != nullptr) {
                block->next->prev = block->prev;
            }

            block->prev = avail_blocks_tail<SlotSize>;
            block->next = nullptr;

            if (avail_blocks_tail<SlotSize> != nullptr) {
                avail_blocks_tail<SlotSize>->next = block;
            }
            avail_blocks_tail<SlotSize> = block;

            if (avail_blocks<SlotSize> == nullptr) {
                avail_blocks<SlotSize> = block;
            }

            // std::cout << "Move block " << block << " to avail_blocks (slots_used: " << block->slots_used << ", ptr: " << ptr << ")" << std::endl;
        }
    }

    template<size_type SlotSize>
    void initialize_block_list() {
        avail_blocks<SlotSize> = nullptr;
        avail_blocks_tail<SlotSize> = nullptr;
        full_blocks<SlotSize> = nullptr;
    }

    template<size_type SlotSize>
    void free_unused_blocks() {
        Block<SlotSize>* curr = avail_blocks<SlotSize>;
        while (curr != nullptr) {
            if (curr->is_empty()) {
                if (avail_blocks<SlotSize> == curr) {
                    avail_blocks<SlotSize> = curr->next;
                }
                if (avail_blocks_tail<SlotSize> == curr) {
                    avail_blocks_tail<SlotSize> = curr->prev;
                }
                if (curr->prev != nullptr) {
                    curr->prev->next = curr->next;
                }
                if (curr->next != nullptr) {
                    curr->next->prev = curr->prev;
                }
                Block<SlotSize>* next = curr->next;
                if (curr->free_list != nullptr) {
                    delete curr;
                }
                curr = next;
            } else {
                curr = curr->next;
            }
        }
    }
};

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator() noexcept {
    initialize_block_list<8>();
    initialize_block_list<16>();
    initialize_block_list<32>();
    initialize_block_list<64>();
    initialize_block_list<128>();
    initialize_block_list<256>();
    initialize_block_list<512>();
    initialize_block_list<1016>();
    initialize_block_list<2032>();
    initialize_block_list<4064>();
}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator& mallocator) noexcept {}

template<typename _Tp, size_t BlockSize>
template<class _Up>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator<_Up>& other) noexcept {}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::~mAllocator() {
    // free_unused_blocks<8>();
    // free_unused_blocks<16>();
    // free_unused_blocks<32>();
    // free_unused_blocks<64>();
    // free_unused_blocks<128>();
    // free_unused_blocks<256>();
    // free_unused_blocks<512>();
    // free_unused_blocks<1016>();
    // free_unused_blocks<2032>();
    // free_unused_blocks<4064>();
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
    size_type slot_size = n * sizeof(value_type);
    if (slot_size <= 8) {
        return reinterpret_cast<pointer>(allocate_slot<8>());
    } else if (slot_size <= 16) {
        return reinterpret_cast<pointer>(allocate_slot<16>());
    } else if (slot_size <= 32) {
        return reinterpret_cast<pointer>(allocate_slot<32>());
    } else if (slot_size <= 64) {
        return reinterpret_cast<pointer>(allocate_slot<64>());
    } else if (slot_size <= 128) {
        return reinterpret_cast<pointer>(allocate_slot<128>());
    } else if (slot_size <= 256) {
        return reinterpret_cast<pointer>(allocate_slot<256>());
    } else if (slot_size <= 512) {
        return reinterpret_cast<pointer>(allocate_slot<512>());
    } else if (slot_size <= 1016) {
        return reinterpret_cast<pointer>(allocate_slot<1016>());
    } else if (slot_size <= 2032) {
        return reinterpret_cast<pointer>(allocate_slot<2032>());
    } else if (slot_size <= 4064) {
        return reinterpret_cast<pointer>(allocate_slot<4064>());
    } else {
        return reinterpret_cast<pointer>(::operator new(n * sizeof(value_type)));
    }
}

template<typename _Tp, size_t BlockSize>
void mAllocator<_Tp, BlockSize>::deallocate(pointer p, size_type n) {
    size_type slot_size = n * sizeof(value_type);
    if (slot_size <= 8) {
        deallocate_slot<8>(p);
    } else if (slot_size <= 16) {
        deallocate_slot<16>(p);
    } else if (slot_size <= 32) {
        deallocate_slot<32>(p);
    } else if (slot_size <= 64) {
        deallocate_slot<64>(p);
    } else if (slot_size <= 128) {
        deallocate_slot<128>(p);
    } else if (slot_size <= 256) {
        deallocate_slot<256>(p);
    } else if (slot_size <= 512) {
        deallocate_slot<512>(p);
    } else if (slot_size <= 1016) {
        deallocate_slot<1016>(p);
    } else if (slot_size <= 2032) {
        deallocate_slot<2032>(p);
    } else if (slot_size <= 4064) {
        deallocate_slot<4064>(p);
    } else {
        ::operator delete(p);
    }
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::size_type
mAllocator<_Tp, BlockSize>::max_size() const noexcept {
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

