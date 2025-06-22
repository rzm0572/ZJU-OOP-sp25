#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <new>
#include <iostream>
#include <cstdlib>

/**
 * @def DEFAULT_ALLOC_SLOT
 * @brief Defines the default number of memory slots to allocate when a free list
 * for a specific size is empty and needs to be replenished.
 */
#ifndef DEFAULT_ALLOC_SLOT
    #define DEFAULT_ALLOC_SLOT 20
#endif

/**
 * @class MallocAllocator
 * @brief A simple, stateless allocator class that wraps the standard C functions
 * `malloc` and `free`. This serves as a fallback for allocations that are too
 * large to be handled by the memory pool of `mAllocator`.
 */
class MallocAllocator {
public:
    /**
     * @brief Allocates a block of memory of the specified size.
     * @param size The number of bytes to allocate.
     * @return A void pointer to the allocated memory, or nullptr if allocation fails.
     */
    static void* allocate(size_t size) {
        void* result = malloc(size);
        return result;
    }

    /**
     * @brief Deallocates a previously allocated block of memory.
     * @param ptr A pointer to the memory block to deallocate.
     */
    static void deallocate(void* ptr) {
        free(ptr);
    }
};

/**
 * @class mAllocator
 * @brief A custom memory allocator that uses a memory pool with fixed-size slots
 * to improve performance for small object allocations.
 *
 * This allocator requests large blocks of memory from the system (using malloc)
 * and then manages this memory itself, dividing it into smaller, fixed-size slots.
 * When an allocation is requested, it serves a slot from a corresponding free list.
 * This can be much faster than calling malloc for every small allocation, as it
 * reduces system call overhead. For allocations larger than a certain threshold (128 bytes),
 * it falls back to using the `MallocAllocator`.
 *
 * @tparam _Tp The type of object to allocate.
 * @tparam BlockSize The size in bytes of the large memory blocks to request from the system.
 */
template<class _Tp, size_t BlockSize = 4096>
class mAllocator {
public:
    // These type aliases are required by the C++ Standard Library Allocator concept.
    // They allow this allocator to be used with STL containers.
    using _Not_user_specialized = void;
    using value_type = _Tp;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type; // The allocator state can be moved.
    using is_always_equal = std::true_type; // All instances of this allocator share the same state (static pool).

    /**
     * @struct rebind
     * @brief A nested template struct required by the Allocator concept. It allows a container
     * to obtain an allocator for a different type. For example, `std::list<T, mAllocator<T>>`
     * can use `rebind` to get an allocator for its internal node type.
     * @tparam _Up The type to rebind the allocator to.
     */
    template <class _Up> struct rebind {
        using other = mAllocator<_Up, BlockSize>;
    };

    // Constructors and Destructor
    mAllocator() noexcept;
    mAllocator(const mAllocator& mallocator) noexcept;
    template<class _Up> mAllocator(const mAllocator<_Up>& other) noexcept;
    ~mAllocator();

    // Member Functions
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
    // --- Configuration Constants ---
    constexpr static size_t BLOCK_SIZE = BlockSize; // The size of memory chunks to request from the OS.
    constexpr static int SLOT_SIZE_NUM = 16;       // The number of fixed-size free lists.

    // Defines the 16 available slot sizes, from 8 to 128 bytes in 8-byte increments.
    constexpr static int SLOT_SIZE_AVAILABLE[SLOT_SIZE_NUM] = {
        8, 16, 24, 32, 40, 48, 56, 64,
        72, 80, 88, 96, 104, 112, 120, 128
    };

    /**
     * @union slot_t
     * @brief Represents a single memory slot in the pool.
     * This union allows a block of memory to be treated either as a pointer to the
     * next free slot (when on a free list) or as raw memory for the user's data.
     * The `data[0]` is a technique to refer to the memory at the start of the struct.
     */
    union slot_t {
        slot_t* next;
        _Tp data[0]; // Flexible array member-like behavior
    };

    /**
     * @brief Maps a requested allocation size to an index in the `free_slot_list` array.
     * @param size The size in bytes of the requested allocation.
     * @return The index (0-15) for the appropriate slot size. Returns -1 if the size
     * is larger than the maximum slot size (128 bytes).
     */
    int get_slot_index(size_t size) {
        return size > 128 ? -1 : (size - 1) >> 3; // Fast division by 8: (size-1)/8
    }

    // --- Static Data Members ---
    // These members are static, meaning they are shared across ALL instances of
    // mAllocator<_Tp, BlockSize>. This creates a single, global memory pool for each
    // allocator type specialization.

    // An array of pointers, where each pointer is the head of a free list for a specific slot size.
    static slot_t* free_slot_list[SLOT_SIZE_NUM];
    
    // Pointers to manage the currently active large memory block obtained from the OS.
    static uint8_t* memory_pool_start; // Start of the available memory in the current block.
    static uint8_t* memory_pool_end;   // End of the current memory block.

    // --- Private Helper Functions ---
    void* extend_free_slot_list(size_type size);
    void* alloc_slot_mempool(size_type size, int count);
    void inflate_mempool(size_type size);
};

// --- Static Member Initialization ---
// Initialize the array of free list heads to null.
template<class _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::slot_t* mAllocator<_Tp, BlockSize>::free_slot_list[SLOT_SIZE_NUM] = {nullptr};

// Initialize the memory pool pointers to null. The pool will be created on the first allocation.
template<class _Tp, size_t BlockSize>
uint8_t* mAllocator<_Tp, BlockSize>::memory_pool_start = nullptr;

template<class _Tp, size_t BlockSize>
uint8_t* mAllocator<_Tp, BlockSize>::memory_pool_end = nullptr;


/**
 * @brief Replenishes a depleted free list for a given slot size.
 * It gets a chunk of memory from the main pool and divides it into
 * `DEFAULT_ALLOC_SLOT` new slots, linking them together into a new free list.
 * @param size The size of the objects needed, used to determine which free list to fill.
 * @return A pointer to the head of the newly created list of slots.
 */
template<class _Tp, size_t BlockSize>
void* mAllocator<_Tp, BlockSize>::extend_free_slot_list(size_type size) {
    int count = DEFAULT_ALLOC_SLOT;
    int slot_index = get_slot_index(size);
    if (slot_index == -1) { // Should not happen if called correctly
        return nullptr;
    }
    int alloc_size = SLOT_SIZE_AVAILABLE[slot_index];

    // Get a contiguous block of memory to be carved into slots.
    slot_t* start_slot = reinterpret_cast<slot_t*>(alloc_slot_mempool(alloc_size, count));
    if (!start_slot) {
        return nullptr;
    }

    // Link the new slots together and add them to the front of the free list.
    slot_t* current_slot = start_slot;
    for (int i = 0; i < count; i++) {
        current_slot->next = free_slot_list[slot_index];
        free_slot_list[slot_index] = current_slot;
        // Move to the beginning of the next slot
        current_slot = reinterpret_cast<slot_t*>(reinterpret_cast<uint8_t*>(current_slot) + alloc_size);
    }
    return free_slot_list[slot_index];
}

/**
 * @brief Allocates a chunk of memory from the main memory pool.
 * If the current pool is insufficient, it triggers an expansion by calling `inflate_mempool`.
 * @param size The size of a single element to allocate.
 * @param count The number of elements to allocate.
 * @return A pointer to the beginning of the allocated chunk.
 */
template<class _Tp, size_t BlockSize>
void* mAllocator<_Tp, BlockSize>::alloc_slot_mempool(size_type size, int count) {
    size_type alloc_size = size * count;
    // Check if there's enough space in the current memory block.
    if (memory_pool_start + alloc_size > memory_pool_end) {
        inflate_mempool(alloc_size); // Not enough space, get more from the OS.
    }

    // Carve out the requested memory by advancing the start pointer.
    slot_t* slot = reinterpret_cast<slot_t*>(memory_pool_start);
    memory_pool_start += alloc_size;
    return slot;
}

/**
 * @brief Requests a new large block of memory from the operating system via `malloc`.
 * This is called when the current memory pool is exhausted.
 * @param size The minimum required size for the new block. The actual allocated size
 * will be rounded up to the nearest multiple of `BLOCK_SIZE`.
 * @throws std::bad_alloc if `malloc` fails.
 */
template<class _Tp, size_t BlockSize>
void mAllocator<_Tp, BlockSize>::inflate_mempool(size_type size) {
    // Calculate the allocation size, rounding up to the nearest BLOCK_SIZE.
    size_type alloc_size = (size + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE;

    memory_pool_start = static_cast<uint8_t*>(malloc(alloc_size));
    if (!memory_pool_start) { // Malloc failed
        memory_pool_end = nullptr;
        throw std::bad_alloc();
    }

    // Update the end pointer for the new block.
    memory_pool_end = memory_pool_start + alloc_size;
}

// --- Public Method Implementations ---

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator() noexcept {
    // Default constructor. No per-instance state to initialize.
}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator& mallocator) noexcept {
    // Copy constructor. No per-instance state to copy.
}

template<typename _Tp, size_t BlockSize>
template<class _Up>
mAllocator<_Tp, BlockSize>::mAllocator(const mAllocator<_Up>& other) noexcept {
    // Generic copy constructor. No per-instance state to copy.
}

template<typename _Tp, size_t BlockSize>
mAllocator<_Tp, BlockSize>::~mAllocator() {
    // Destructor. Memory is managed statically and typically not released until
    // program termination. A more robust implementation might have a static
    // destructor to free all allocated blocks.
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::pointer mAllocator<_Tp, BlockSize>::address(reference x) const noexcept {
    return &x;
}

template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::const_pointer mAllocator<_Tp, BlockSize>::address(const_reference x) const noexcept {
    return &x;
}

/**
 * @brief Allocates memory for `n` objects of type `_Tp`.
 * @param n The number of objects to allocate space for.
 * @param hint An optional pointer that can be used by the allocator to improve locality. (Not used here).
 * @return A pointer to the allocated memory.
 */
template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::pointer mAllocator<_Tp, BlockSize>::allocate(size_type n, const _Not_user_specialized* hint) {
    int alloc_size = n * sizeof(value_type);
    int index = get_slot_index(alloc_size);

    // If the requested size is too large for the slot system, fall back to malloc.
    if (index == -1) {
        return reinterpret_cast<pointer>(MallocAllocator::allocate(alloc_size));
    } else {
        // If the appropriate free list is empty, replenish it.
        if (free_slot_list[index] == nullptr) {
            extend_free_slot_list(alloc_size);
        }
        // Pop a slot from the front of the free list.
        slot_t* slot = free_slot_list[index];
        free_slot_list[index] = slot->next;
        return reinterpret_cast<pointer>(slot);
    }
}

/**
 * @brief Deallocates memory for `n` objects previously allocated by `allocate`.
 * @param p A pointer to the memory to deallocate.
 * @param n The number of objects for which space was allocated.
 */
template<typename _Tp, size_t BlockSize>
void mAllocator<_Tp, BlockSize>::deallocate(pointer p, size_type n) {
    if (p == nullptr) {
        return; // Deallocating null is a no-op.
    }

    int free_size = n * sizeof(value_type);
    int index = get_slot_index(free_size);

    // If it was a large allocation, fall back to free.
    if (index == -1) {
        MallocAllocator::deallocate(static_cast<void*>(p));
    } else {
        // Otherwise, cast the memory back to a slot and add it to the front of the
        // corresponding free list.
        slot_t* slot = reinterpret_cast<slot_t*>(p);
        slot->next = free_slot_list[index];
        free_slot_list[index] = slot;
    }
}

/**
 * @brief Returns the maximum number of elements that could theoretically be allocated.
 */
template<typename _Tp, size_t BlockSize>
typename mAllocator<_Tp, BlockSize>::size_type mAllocator<_Tp, BlockSize>::max_size() const noexcept {
    // The maximum value of size_type divided by the size of a single element.
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

/**
 * @brief Constructs an object in the given memory location.
 * Uses placement new to construct an object of type `_Up` at the address `p`.
 * @tparam _Up The type of the object to construct.
 * @tparam Args The types of arguments for the constructor.
 * @param p Pointer to the uninitialized memory.
 * @param args Arguments to forward to the object's constructor.
 */
template<typename _Tp, size_t BlockSize>
template<class _Up, class... Args>
void mAllocator<_Tp, BlockSize>::construct(_Up* p, Args&&... args) {
    // Use placement new to construct the object in the provided memory.
    ::new (static_cast<void*>(p)) _Up(std::forward<Args>(args)...);
}

/**
 * @brief Destroys an object without deallocating its memory.
 * Calls the destructor of the object at address `p`.
 * @tparam _Up The type of the object to destroy.
 * @param p Pointer to the object to be destroyed.
 */
template<typename _Tp, size_t BlockSize>
template<class _Up>
void mAllocator<_Tp, BlockSize>::destroy(_Up* p) {
    if (p != nullptr) {
        // Explicitly call the destructor.
        p->~_Up();
    }
}