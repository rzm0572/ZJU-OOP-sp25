#pragma once

#include <cstddef>

template<class _Tp, size_t BlockSize = 4096>
class bAllocator{
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
        using other = bAllocator<_Up, BlockSize>;
    };

    bAllocator() noexcept;
    bAllocator(const bAllocator& mallocator) noexcept;
    template<class _Up> bAllocator(const bAllocator<_Up>& other) noexcept;
    ~bAllocator();

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

};

template<typename _Tp, size_t BlockSize>
bAllocator<_Tp, BlockSize>::bAllocator() noexcept {

}

template<typename _Tp, size_t BlockSize>
bAllocator<_Tp, BlockSize>::bAllocator(const bAllocator& mallocator) noexcept {

}

template<typename _Tp, size_t BlockSize>
template<class _Up>
bAllocator<_Tp, BlockSize>::bAllocator(const bAllocator<_Up>& other) noexcept {

}

template<typename _Tp, size_t BlockSize>
bAllocator<_Tp, BlockSize>::~bAllocator() {

}

template<typename _Tp, size_t BlockSize>
typename bAllocator<_Tp, BlockSize>::pointer
bAllocator<_Tp, BlockSize>::address(reference x) const noexcept {

}

template<typename _Tp, size_t BlockSize>
typename bAllocator<_Tp, BlockSize>::const_pointer
bAllocator<_Tp, BlockSize>::address(const_reference x) const noexcept {

}

template<typename _Tp, size_t BlockSize>
typename bAllocator<_Tp, BlockSize>::pointer
bAllocator<_Tp, BlockSize>::allocate(size_type n, const _Not_user_specialized* hint) {

}

template<typename _Tp, size_t BlockSize>
void bAllocator<_Tp, BlockSize>::deallocate(pointer p, size_type n) {

}

template<typename _Tp, size_t BlockSize>
typename bAllocator<_Tp, BlockSize>::size_type
bAllocator<_Tp, BlockSize>::max_size() const noexcept {

}

template<typename _Tp, size_t BlockSize>
template<class _Up, class... Args>
void bAllocator<_Tp, BlockSize>::construct(_Up* p, Args&&... args) {

}

template<typename _Tp, size_t BlockSize>
template<class _Up>
void bAllocator<_Tp, BlockSize>::destroy(_Up* p) {

}
