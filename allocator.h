//
// Created by oem on 23.05.18.
//

#pragma once

#include <type_traits>
#include <memory>
#include <stack>
#include <list>
#include <vector>
#include <iostream>

namespace kkk
{

template<typename T>
struct FixedAllocator {
public:
    FixedAllocator();
    ~FixedAllocator();

    using element_type = T;
    using void_pointer = void*;
    using size_type = std::size_t;

    void_pointer allocate();
    void deallocate(void_pointer);


private:

    size_type size_block;
    size_type num_block;
    struct block {
        struct block *prev = nullptr, *next  = nullptr;
    };

    using stack_pool = void**;
    stack_pool pools;
    size_type size_pool;
    size_type max_size_pool;

    using block_pointer  = struct block*;
    block_pointer head_free_blocks;
    block_pointer head_allocated_blocks;

    void add_pool();
    void pop_front(block_pointer& head) {
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
    }

    void push_front(block_pointer& head, block_pointer& block) {
        block->prev = nullptr;
        block->next = head;
        if (head != nullptr) {
            head->prev = block;
        }
        head = block;
    }

    void delete_block(block_pointer& head, block_pointer& block) {
        block_pointer next = block->next;
        block_pointer prev = block->prev;
        if (next != nullptr) {
            next->prev = prev;
        }

        if (prev == nullptr) {
            head = nullptr;
        } else {
            prev->next = next;
        }
    }

    size_type size_(block_pointer head) {
        size_type s = 0;
        for (;head != nullptr; head = head->next) { ++s; }
        return s;
    }
};

template <typename T>
FixedAllocator<T>::FixedAllocator()
{
    size_block = sizeof(T)  + sizeof(block);
    num_block = 65550;
    head_free_blocks = nullptr;
    head_allocated_blocks = nullptr;

    max_size_pool = 50;
    size_pool = 0;
    pools = new void*[max_size_pool];

}

template <typename T>
void FixedAllocator<T>::add_pool() {
    void_pointer pool = ::operator new(num_block * size_block);
    if (pool == nullptr || size_pool == max_size_pool) {
        throw std::bad_alloc();
    }

    pools[size_pool++] = (pool);

    for (size_type i = 0; i < num_block; ++i) {
        auto block = reinterpret_cast<block_pointer>(static_cast<char*>(pool) + i * size_block);
        this->push_front(head_free_blocks, block);

    }
//    std::cout << "Log: add pool " << std::endl
//              << "count pool:" << pools.size() << std::endl
//              << " num free block " << this->size_(head_free_blocks) << std::endl;
}

template <typename T>
FixedAllocator<T>::~FixedAllocator() {
//    std::cout << "Log: delete " << std::endl
//              << "count pool:" << pools.size() << std::endl
//              << " num free block " << this->size_(head_free_blocks) << std::endl
//              << " num allocated block " << this->size_(head_allocated_blocks) << std::endl;

    for (int i = 0; i < size_pool; ++i) {
        auto pool = static_cast<void_pointer>(pools[i]);
        ::operator delete(pool);
    }

    if (head_allocated_blocks != nullptr) {
        throw std::exception();
    }
}


template <typename T>
typename FixedAllocator<T>::void_pointer FixedAllocator<T>::allocate() {
    if (head_free_blocks == nullptr) {
        this->add_pool();
    }

    block_pointer block = head_free_blocks;
    pop_front(head_free_blocks);
    push_front(head_allocated_blocks, block);

//    std::cout << "Log: allocate " << std::endl
//              << " num free block " << this->size_(head_free_blocks) << std::endl
//              << " num allocated block " << this->size_(head_allocated_blocks) << std::endl;

    return static_cast<void_pointer>(reinterpret_cast<char*>(block) + sizeof(FixedAllocator<T>::block));
}

template <typename T>
void FixedAllocator<T>::deallocate(void_pointer p) {
    auto block = reinterpret_cast<block_pointer>(static_cast<char*>(p) - sizeof(FixedAllocator<T>::block));

    delete_block(head_allocated_blocks, block);
    push_front(head_free_blocks, block);

//    std::cout << "Log: deallocate " << std::endl
//              << " num free block " << this->size_(head_free_blocks) << std::endl
//              << " num allocated block " << this->size_(head_allocated_blocks) << std::endl;
}


template <typename T>
class Allocator
{
private:
    static FixedAllocator<T> allocator_policy;

public:

    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    template <class U> struct rebind { using other = Allocator<U>; };

    constexpr Allocator() noexcept = default;
    constexpr Allocator( const Allocator& other ) noexcept {}
    template <class U>
    constexpr Allocator(const Allocator<U>& other) noexcept {}
    Allocator &operator=(const Allocator&) = delete;

    ~Allocator() = default;

    [[nodiscard]] pointer allocate(size_type n) {
        if (n == 1) {
            return static_cast<pointer>(Allocator<T>::allocator_policy.allocate());
        }
//       return reinterpret_cast<pointer>(::operator new(sizeof (T)));
        throw std::bad_alloc();
    }

    void deallocate(pointer p, size_type n) {
        if (n == 1) {
            Allocator<T>::allocator_policy.deallocate(p);
            return;
        }
//        ::operator delete(p);
        throw std::bad_alloc();
    }

    template <typename U, typename ...Args>
    void construct(U* p, Args &&...args) {
        new (p) U(std::forward<Args>(args)...);
    };

    template <typename U>
    void destroy(U* p) {
        p->~U();
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

};

template <typename T> FixedAllocator<T> Allocator<T>::allocator_policy;


template< class T1, class T2 >
bool operator==(const Allocator<T1>& lhs, const Allocator<T2>& rhs ) {
    return true;
};

template <class T1, class OtherAllocator>
bool operator==(const Allocator<T1>& lhs, const OtherAllocator& rhs) {
    return false;
};

template <class T1, class T2>
bool operator!=(const Allocator<T1>& lhs, const Allocator<T2>& rhs) {
    return !(lhs == rhs);
};


} // kkk




template <typename T>
struct std::allocator_traits<kkk::Allocator<T>> {
    using allocator_type = typename kkk::Allocator<T>;
    using value_type = typename kkk::Allocator<T>::value_type;
    using pointer = typename kkk::Allocator<T>::pointer;

    using const_pointer = typename std::pointer_traits<pointer>::template rebind<const value_type>;
    using void_pointer = typename std::pointer_traits<pointer>::template rebind<void>;
    using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;

    using difference_type = typename std::pointer_traits<pointer>::difference_type;
    using size_type = typename std::make_unsigned<difference_type>::type;

    using propagate_on_container_copy_assignment = std::false_type;
//    true if the allocator of type A needs to be copied when the container that uses it is copy-assigned.
// Note that if the allocators of the source and the target containers do not compare equal, copy assignment has
// to deallocate the target's memory using the old allocator and then allocate it using the new allocator before
// copying the elements (and the allocator).
    using propagate_on_container_move_assignment = std::false_type;
//    true if the allocator of type A needs to be moved when the container that uses it is move-assigned.
// If this member is false and the allocators of the source and the target containers do not compare equal,
// move assignment cannot take ownership of the source memory and must move-assign or move-construct the
// elements individually, resizing its own memory as needed.
    using propagate_on_container_swap = std::false_type;
//    true if the allocators of type A need to be swapped when two containers that
// use them are swapped. If this member is false and the allocators of the two containers
// do not compare equal, the behavior of container swap is undefined.

    using is_always_equal = std::false_type;

    template <class U> using rebind_alloc = kkk::Allocator<U>;
    template <class U> using rebind_traits = std::allocator_traits<rebind_alloc<U>>;


    [[nodiscard]] static pointer allocate(allocator_type& a, size_type n ) {
        return a.allocate(n);
    }

    [[nodiscard]] static pointer allocate(allocator_type& a, size_type n,
                                          const_void_pointer hint) {
        return a.allocate(n);
    }

    static void deallocate (allocator_type &a, pointer p, size_type n) {
        a.deallocate(p, n);
    }

    template<typename U , typename... Args>
    static void construct(allocator_type &a, U* p, Args &&...args) {
        a.construct(p, std::forward<Args>(args)...);
    };

    template <typename U>
    static void destroy(allocator_type &a, U* p) {
        a.destroy(p);
    }

    static size_type max_size (const allocator_type &a) {
        return a.max_size();
    }

    static allocator_type select_on_container_copy_construction (const allocator_type &rhs) {
        return allocator_type(rhs);
    }
};
