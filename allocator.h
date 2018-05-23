//
// Created by oem on 23.05.18.
//

#pragma once

#include <type_traits>
#include <memory>

namespace kkk
{

template <typename T>
class Allocator
{
    using rebind = template<class U> struct rebind { typedef Allocator<U> other; };

    constexpr Allocator() noexcept;
    constexpr Allocator( const Allocator& other ) noexcept;

    template <class U>
    constexpr Allocator(const Allocator <U>& other) noexcept;

    ~Allocator();
    [[nodiscard]] T* Allocate( std::size_t n ); // Throws std::bad_alloc if allocation fails.
    void deallocate( T* p, std::size_t n );

};

template< class T1, class T2 >
bool operator==( const Allocator<T1>& lhs, const Allocator<T2>& rhs );
//  Since default allocators are stateless, two default allocators are always equal.
template< class T1, class T2 >
bool operator!=( const Allocator<T1>& lhs, const Allocator<T2>& rhs );


} // kkk



template <typename T>
struct allocator_traits<kkk::Allocator<T>> {
    using allocator_type = kkk::Allocator<T>;
    using value_type = T;
    using pointer = value_type*;

    using const_pointer = std::pointer_traits<pointer>::rebind<const value_type>;
    using void_pointer = std::pointer_traits<pointer>::rebind<void>;
    using const_void_pointer = std::pointer_traits<pointer>::rebind<const void>;
    using difference_type = std::pointer_traits<pointer>::difference_type;
    using size_type = std::make_unsigned<difference_type>::type;

    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;

    using is_always_equal = std::is_empty<allocator_type>::type;

    using rebind_alloc<T> = allocator_type::rebind<T>::other;
    using rebind_traits<T> = std::allocator_traits<rebind_alloc<T>>;


    [[nodiscard]] static pointer allocate( Alloc& a, size_type n ) {

    }
    [[nodiscard]] static pointer allocate( Alloc& a, size_type n,
                                           const_void_pointer hint ); // n*sizeof(Alloc::value_type)

    template<typename _Tp , typename... _Args>
    static void construct (allocator_type &__a, _Tp *__p, _Args &&...__args);

    static void deallocate (allocator_type &__a, pointer __p, size_type __n);

    template<class _Tp >
    static void 	destroy (allocator_type &__a, _Tp *__p);

    static size_type 	max_size (const allocator_type &__a);
    static allocator_type 	select_on_container_copy_construction (const allocator_type &__rhs);
};



