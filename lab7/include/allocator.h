#include<stdio.h>
#include<cstddef>
#include<type_traits>
#include<iostream>
#define BLOCK_SIZE 64
#define ENTRY_SIZE 128


template<typename T>
class Allocator {
public:
    typedef void _Not_user_specialized;//定义_Not_user_specialized为void类型
    typedef T value_type;//定义类型为T类型
    typedef value_type* pointer;//定义指针为value_type*类型
    typedef const value_type* const_pointer;//定义常量指针为const value_type*类型
    typedef value_type& reference;//定义引用为value_type&类型
    typedef const value_type& const_reference;//定义常量引用为const value_type&类型
    typedef std::size_t size_type;//表示内存块大小的无符号整型
    typedef std::ptrdiff_t difference_type;//表示指针距离的有符号整型
    typedef std::true_type propagate_on_container_move_assignment;//指定容器移动赋值时允许传播分配器
    typedef std::true_type is_always_equal;//分配器总是与其他实例相等

    template<typename U>
    struct rebind {//定义rebind类型，用于分配并非STL容器储存元素类型的元素
        typedef Allocator<U> other;
    };


    Allocator(){
        // FirstEntry = nullptr;
        // FreeBlock = nullptr;
        newEntry();
    };//构造函数
    ~Allocator(){
        // Entry* p = FirstEntry;
        // while(p!= nullptr){
        //     Entry* next = p->next;
        //     delete[] p;
        //     p = next;
        // }
    }//析构函数
    pointer address(reference x) const { return &x; }//返回x的地址
    const_pointer address(const_reference x) const { return &x; }//返回x的常量地址
    pointer allocate(size_type n, const void* hint = 0) {
        // return static_cast<pointer>(::operator new(n * sizeof(value_type)));
        return static_cast<pointer>(allocateBlock(n));
    
    }//分配内存块
    pointer allocate_at_least(size_type n, const void* hint = 0) {return allocate(n);}
    void deallocate(pointer p, size_type n){
        // ::operator delete(p);
        if(n*sizeof(value_type) > BLOCK_SIZE){
            ::operator delete(p);
        }
        else{
            deallocateBlock(p);
        }
    } //释放内存块
    size_type max_size() const {return size_t(-1);}//返回最大内存块大小(need to be changed)
    bool operator==(const Allocator&) const { return true; }
    bool operator!=(const Allocator&) const { return false; }
    void construct(pointer p, const T& value) { new(p) T(value);}
    void destroy(pointer p){p->~T();}//销毁对象

    const size_t BlockNum = ENTRY_SIZE/sizeof(Block);
    const size_t ElementNum = BLOCK_SIZE/sizeof(value_type);

    union Block{//储存元素的最小内存块
        char data[BLOCK_SIZE];//储存数据
        Block* next;//指向下一个Block指针
    };
    
    struct Entry{//每次分配的内存块大小为ENTRY_SIZE
        Block* block;
        Entry* next;//下一个Entry指针
    };
    static Entry* FirstEntry;
    static Block* FreeBlock;

    void newEntry(){//创建新的Entry
        Entry* newEntry = new Entry;
        //先将 Entrys 链接起来
        newEntry->next = FirstEntry;
        FirstEntry = newEntry;
        //分配足够的空间
        newEntry->block = static_cast<Block*>(::operator new(ENTRY_SIZE));
        //初始化每个block，即将每个block的next指针指向第一个空出的block
        for(size_t i=0;i<BlockNum;i++){
            Block* block = newEntry->block+i;
            block->next = FreeBlock;
            FreeBlock = block;
        }
    }

    void* allocateBlock(size_t n){//分配新的内存块
        if(n>ElementNum){//如果要分配的元素数量大于一个block的容量，则不使用memory pool
            return (::operator new(n * sizeof(value_type)));
        }
        if(FreeBlock == nullptr){//如果没有空闲的block，则创建一个新的Entry
            newEntry();
        }
        Block* currentBlock = FreeBlock;
        FreeBlock = FreeBlock->next; // 移动空闲链表头指针
        return static_cast<void*>(currentBlock->data); // 显式转换为用户类型指针
    }

    void deallocateBlock(pointer p){//释放内存块
        Block* block = reinterpret_cast<Block*>(p); 
        block->next = FreeBlock;
        FreeBlock = block;
    }

    // void print_info(){
        
    //     Entry* p = FirstEntry;
    //     int count = 0;
    //     while(p!= nullptr){
    //         count++;
    //         printf("Entry %d: ", count);
    //         Block* block = p->block;
    //         for(size_t i=0;i<BlockNum;i++){
    //             Block* currentBlock = block+i;
    //             printf("%p ", currentBlock->data);
    //         }
    //         printf("\n");
    //         p = p->next;
    //     }
    //     std::cout<<"total entries: "<<FirstEntry<<std::endl;
    // }
};



// 类外定义静态成员（关键修正点）
template<typename T>
typename Allocator<T>::Entry* Allocator<T>::FirstEntry = nullptr;

template<typename T>
typename Allocator<T>::Block* Allocator<T>::FreeBlock = nullptr;