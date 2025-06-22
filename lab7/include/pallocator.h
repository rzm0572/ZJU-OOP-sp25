#include <cstddef>
#include <stdio.h>
#include <type_traits>
template <typename T> class pAllocator {
public:
  typedef void _Not_user_specialized; // 定义_Not_user_specialized为void类型
  typedef T value_type;               // 定义类型为T类型
  typedef value_type *pointer;        // 定义指针为value_type*类型
  typedef const value_type *const_pointer; // 定义常量指针为const
                                           // value_type*类型
  typedef value_type &reference; // 定义引用为value_type&类型
  typedef const value_type
      &const_reference;          // 定义常量引用为const value_type&类型
  typedef std::size_t size_type; // 表示内存块大小的无符号整型
  typedef std::ptrdiff_t difference_type; // 表示指针距离的有符号整型
  typedef std::true_type
      propagate_on_container_move_assignment; // 指定容器移动赋值时允许传播分配器
  typedef std::true_type is_always_equal; // 分配器总是与其他实例相等

  template <typename U>
  struct rebind { // 定义rebind类型，用于分配并非STL容器储存元素类型的元素
    typedef pAllocator<U> other;
  };

  pAllocator() = default;                           // 构造函数
  ~pAllocator() = default;                          // 析构函数
  pointer address(reference x) const { return &x; } // 返回x的地址
  const_pointer address(const_reference x) const {
    return &x;
  } // 返回x的常量地址
  pointer allocate(size_type n, const void *hint = 0) {
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
  } // 分配内存块
  pointer allocate_at_least(size_type n, const void *hint = 0) {
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
  }
  void deallocate(pointer p, size_type n) { ::operator delete(p); } // 释放内存块
  size_type max_size() const {
    return size_t(-1);
  } // 返回最大内存块大小(need to be changed)
  bool operator==(const pAllocator &) const { return true; }
  bool operator!=(const pAllocator &) const { return false; }
  void construct(pointer p, const T &value) { new (p) T(value); }
  void destroy(pointer p) { p->~T(); } // 销毁对象
};
