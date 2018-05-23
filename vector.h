//
// Created by oem on 24.04.18.
//
#pragma once

#include <cstring>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <ctgmath>


namespace kkk
{

template <typename T, typename Allocator = std::allocator<T>> class Vector;
template <typename T, bool is_const_iterator> class VectorIterator;

} // kkk

template <typename T, bool is_const_iterator>
struct std::iterator_traits<kkk::VectorIterator<T, is_const_iterator>>{
    using size_type = int;
    using difference_type = std::ptrdiff_t ;
    using value_type = typename std::conditional<is_const_iterator, const T, T>::type;
    using reference = typename std::conditional<is_const_iterator, const T&, T&>::type;
    using pointer = typename std::conditional<is_const_iterator, const T*, T*>::type;
    using iterator_category = std::random_access_iterator_tag ;
};

namespace kkk
{

template <typename T, bool is_const_iterator>
class VectorIterator
{
public:
    using size_type = int;

    VectorIterator() = delete;

    VectorIterator(const VectorIterator& other) : ptr_data_(other.ptr_data_) {}
    VectorIterator(VectorIterator&& other) noexcept : ptr_data_(std::move(other.ptr_data_)) {}

    ~VectorIterator() = default;

    VectorIterator& operator=(const VectorIterator& other) {
        ptr_data_ = other.ptr_data_;
        return *this;
    }

    VectorIterator& operator=(VectorIterator&& other) {
        std::swap(ptr_data_, other.ptr_data_);
        return *this;
    }

    typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::reference operator*() const {
        return *ptr_data_;
    }

    typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::pointer operator->() const {
        return ptr_data_;
    }

    typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::reference operator[](size_type n) const {
        return *(ptr_data_ + n);
    }

    typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::pointer data() const {
        return ptr_data_;
    }

    VectorIterator<T, is_const_iterator>& operator++() {
        ptr_data_++;
        return *this;
    }

    VectorIterator& operator--() {
        ptr_data_--;
        return *this;
    }

    VectorIterator operator++(int) {
        VectorIterator tmp(ptr_data_);
        ptr_data_++;
        return tmp;
    }

    VectorIterator operator--(int) {
        VectorIterator tmp(ptr_data_);
        ptr_data_--;
        return tmp;
    }

    VectorIterator& operator+=(size_type n) {
        ptr_data_ += n;
        return *this;
    }

    VectorIterator& operator-=(size_type n)  {
        ptr_data_ -= n;
        return *this;
    }

    void swap(VectorIterator& b) {
        std::swap(this->ptr_data_, b.ptr_data_);
    }

    typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::difference_type // // must a+n==b, b == a + (b - a)
    friend operator-(const VectorIterator& lv, const VectorIterator& rv)
    {
        return lv.ptr_data_ - rv.ptr_data_;
    }


    friend VectorIterator<T, is_const_iterator> operator+(const VectorIterator<T, is_const_iterator>& iter, int n)
    {
        return n + iter;
    }

    friend VectorIterator<T, is_const_iterator> operator+(int n, const VectorIterator<T, is_const_iterator>& iter)
    {
        return VectorIterator<T, is_const_iterator>(iter.ptr_data_ + n);
    }

    friend VectorIterator<T, is_const_iterator> operator-(const VectorIterator<T, is_const_iterator> & iter, int n)
    {
        return n - iter;
    }

    friend VectorIterator<T, is_const_iterator> operator-(int n, const VectorIterator<T, is_const_iterator>& iter)
    {
        return VectorIterator<T, is_const_iterator>(iter.ptr_data_ - n);
    }


    // const_iterator cast
    operator VectorIterator<T, true>() const {
        return VectorIterator<T, true>(this->ptr_data_);
    };

    VectorIterator(const typename
                   std::iterator_traits<VectorIterator<T, is_const_iterator>>::pointer& ptr_data)
        : ptr_data_(ptr_data) {}

private:
    typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::pointer ptr_data_;
};

template <typename T, bool type>
bool operator==(const VectorIterator<T, type>& this_, const VectorIterator<T, type>& other) {
    return this_.data() == other.data();
}

template <typename T, bool type>
bool operator!=(const VectorIterator<T, type>& this_, const VectorIterator<T, type>& other) {
    return this_.data() != other.data();
}

template <typename T, bool type>
bool operator<(const VectorIterator<T, type>& a, const VectorIterator<T, type>& b) {  // !(a < a), if a < b then !(b < a), if a < b and b < c then a < c, a < b or b < a or a == b
    return a.data() < b.data();
}

template <typename T, bool type>
bool operator>(const VectorIterator<T, type>& a, const VectorIterator<T, type>& b) {
    return b < a;
}

template <typename T, bool type>
bool operator<=(const VectorIterator<T, type>& a, const VectorIterator<T, type>& b) {
    return !(b < a);
}

template <typename T, bool type>
bool operator>=(const VectorIterator<T, type>& a, const VectorIterator<T, type>& b) {
    return !(a < b);
}



template <typename T, typename A>
class Vector
{

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    using iterator = kkk::VectorIterator<T, false>;
    using const_iterator = kkk::VectorIterator<T, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    using allocator_type = A;

    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;


    // construct/copy/destroy:
    Vector() noexcept(noexcept(allocator_type()));
    explicit Vector( const allocator_type& alloc ) noexcept;
    explicit Vector(size_type, const allocator_type& alloc = allocator_type());

    Vector(size_t n, const T& value, const allocator_type& alloc = allocator_type());

    template<typename InputIterator, class = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
    Vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());

    Vector(const Vector<T, A>&);
    Vector(const Vector<T, A>&, const allocator_type& alloc);
    Vector(Vector<T, A>&&) noexcept;
    Vector(Vector<T, A>&&, const allocator_type& alloc) noexcept;
    Vector(std::initializer_list<T>, const allocator_type& alloc = allocator_type());

    ~Vector();

    Vector<T, A>& operator=(Vector<T, A>);
    Vector<T, A>& operator=(Vector<T, A>&&) noexcept;
    Vector& operator=(std::initializer_list<T>) noexcept;

    template<typename InputIterator, class = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
    void assign(InputIterator, InputIterator);

    void assign(size_type, const T&);
    void assign(std::initializer_list<T>);

    allocator_type get_allocator() const noexcept;

    // // iterators:
    iterator begin() noexcept {
        return iterator(ptr_data_);
    }

    const_iterator begin() const noexcept {
        return const_iterator(ptr_data_);
    }

    iterator end() noexcept {
        return iterator(ptr_data_ + size_);
    }

    const_iterator end() const noexcept {
        return const_iterator(ptr_data_ + size_);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(this->end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(this->begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(this->begin());
    }

    const_iterator cbegin() noexcept {
        return const_iterator(ptr_data_);
    }

    const_iterator cend() noexcept {
        return const_iterator(ptr_data_ + size_);
    }

    const_reverse_iterator crbegin() const noexcept {
       return const_reverse_iterator(this->end());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(this->begin());
    }

    // // capacity:
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void resize(size_type);
    void resize(size_type, const T&);
    size_type capacity() const noexcept;


    bool empty() const  noexcept;
    void reserve(size_type);
    void shrink_to_fit();

    // // element access:
    reference operator[](size_type i);
    const_reference operator[](size_type i) const;

    reference at(size_type);
    const_reference at(size_type n) const;

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    // //data access
    T* data() noexcept {
        return this->empty() ? nullptr : ptr_data_;
    }

    const T* data() const noexcept {
        return this->empty() ? nullptr : ptr_data_;
    }

    // // modifiers:
    template <typename... Args> void emplace_back(Args&&... args);
    void push_back(const T&);
    void push_back(T&&);
    void pop_back();

    template <typename... Args> iterator emplace(const_iterator position, Args&&... args);
    iterator insert(const_iterator position, const T& x);
    iterator insert(const_iterator position, T&& x);
    iterator insert(const_iterator position, size_type n, const T& x);

    template<typename InputIterator, class = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
    iterator insert (const_iterator position, InputIterator first, InputIterator last);

    iterator insert(const_iterator position, std::initializer_list<T> values) {
        return this->insert(position, values.begin(), values.end());
    }

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    void swap(Vector<T, A>&) noexcept;
    void clear() noexcept;

private:
    allocator_type allocator_;
    size_type size_;
    size_type capacity_;

    const double GROWTH_FACTOR = 2;
    const double LOW_FACTOR = 0.5;

    pointer ptr_pool_;
    pointer ptr_data_;

    bool requires_reallocation(size_type new_size) { return new_size >= capacity_; }
    void reallocation(size_type);

    void shift_right(size_type n, iterator from, iterator to);
    void shift_left(size_type n, iterator from, iterator to);

    template <typename TypeIteratorBase>
    void validate(TypeIteratorBase base, size_type n, const value_type& value);

    template <typename TypeIteratorBase>
    void validate(TypeIteratorBase base, size_type n);

    template <typename TypeIteratorBase, typename TypeIteratorCon,
        class = typename std::enable_if<!std::is_integral<TypeIteratorCon>::value>::type>
    void validate(TypeIteratorBase base, const TypeIteratorCon& begin, const TypeIteratorCon& end);

    template <typename TypeIterator>
    void invalidate(TypeIterator begin, TypeIterator end);
    void invalidate_all();
};

template <typename T, typename Allocator>
void Vector<T, Allocator>::shift_left(size_type n, iterator from, iterator to) { //  [first, last)
    if (from >= to || n == 0) {
        return;
    }
    std::move(from, to, from - n); // начинается d_first
}

template <typename T, typename A>
void Vector<T, A>::shift_right(size_type n, iterator from, iterator to) { // [begin, last)
    if (from >= to || n == 0) {
        return;
    }
    iterator d_last = to, it_last = to + n, it_first = this->end();
    if (from + n > it_first) {
        it_first = from + n;
    }
    if (it_last > this->end()){
        while (it_last != it_first) {
            new ((--it_last).data()) T(std::move(*(--d_last)));
        }
    }

    if (it_last <= this->end()) {
        std::move_backward(from, d_last, it_last); // [first, last)
    }
}


template <typename T, typename A>
bool operator==(const Vector<T, A>& a, const Vector<T, A>& b) {
    if (a.size() != b.size()) return false;
    for (unsigned i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

template <typename T, typename A>
bool operator!=(const Vector<T, A>& a, const Vector<T, A>& b) {
    return !(a == b);
}

template <typename T, typename A>
bool operator<(const Vector<T, A>& a, const Vector<T, A>& b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template <typename T, typename A>
bool operator> (const Vector<T, A>& a, const Vector<T, A>& b) {
    return b < a;
}

template <typename T, typename A>
bool operator>=(const Vector<T, A>& a, const Vector<T, A>& b) {
    return !(a < b);
}
template <typename T, typename A>
bool operator<=(const Vector<T, A>& a, const Vector<T, A>& b) {
    return !(b < a);
}

template <typename T, typename A>
void swap(Vector<T, A>& first, Vector<T, A>& second) noexcept {
    first.swap(second);
}

} // kkk


template <typename T, typename Allocator>
template <typename TypeIteratorBase,  typename TypeIteratorCon, typename type>
inline void kkk::Vector<T, Allocator>::validate(TypeIteratorBase base,
                                                const TypeIteratorCon& begin,
                                                const TypeIteratorCon& end)
{
    for (auto iter_init = begin; iter_init != end; ++iter_init, ++base) {
        new (base.data()) T(*iter_init);
    }
}

template <typename T, typename Allocator>
template <typename TypeIterator>
void kkk::Vector<T, Allocator>::validate(TypeIterator base,
                                         size_type n,
                                         const value_type& value)
{
    for (size_type i = 0; i < n; ++i, ++base) {
        new (base.data()) T(value);
    }
}

template <typename T, typename Allocator>
template <typename TypeIterator>
void kkk::Vector<T, Allocator>::validate(TypeIterator base, size_type n) {
    for (size_type i = 0; i < n; ++i) {
        new (base.data()) T();
        ++base;
    }
}

template <typename T, typename Allocator>
template <typename TypeIterator>
inline void kkk::Vector<T, Allocator>::invalidate(TypeIterator begin, TypeIterator end) {
    for (auto iter_init = begin; iter_init != end; ++iter_init) {
        (*iter_init).~value_type();
    }
}

template <typename T, typename Allocator>
inline void kkk::Vector<T, Allocator>::invalidate_all() {
    this->invalidate(this->begin(), this->end());
}

template <typename T, typename Allocator>
void kkk::Vector<T, Allocator>::reallocation(size_type new_size) {
    if (this->requires_reallocation(new_size)) {
        size_type needed_capacity = capacity_ + new_size - size_;
        size_type new_capacity = std::ceil(needed_capacity * GROWTH_FACTOR);
        this->reserve(new_capacity);
    }
}

template <typename T, typename A>
kkk::Vector<T, A>::Vector() noexcept(noexcept(A()))
    : allocator_(),
      size_(0),
      capacity_(size_),
      ptr_pool_(std::allocator_traits<A>::allocate(allocator_, capacity_)),
      ptr_data_(ptr_pool_)
{};


template <typename T, typename A>
kkk::Vector<T, A>::Vector(const allocator_type& alloc) noexcept
    : allocator_(alloc),
      size_(0),
      capacity_(size_),
      ptr_pool_(std::allocator_traits<A>::allocate(allocator_, capacity_)),
      ptr_data_(ptr_pool_)
{}

template <typename T, typename A>
kkk::Vector<T, A>::Vector(size_t n, const allocator_type& alloc)
    : allocator_(alloc),
      size_(n),
      capacity_(size_),
      ptr_pool_(std::allocator_traits<A>::allocate(allocator_, capacity_)),
      ptr_data_(ptr_pool_)
{
    this->validate(iterator(ptr_data_), n);
}

template <typename T, typename A>
kkk::Vector<T, A>::Vector(size_t n, const T& value,
                          const allocator_type& alloc)
    : allocator_(alloc),
      size_(n),
      capacity_(size_),
      ptr_pool_(std::allocator_traits<A>::allocate(allocator_, capacity_)),
      ptr_data_(ptr_pool_)
{
    this->validate(iterator(ptr_data_), size_, value);
}

template <typename T, typename A>
template <typename InputIterator, typename enable_if>
kkk::Vector<T, A>::Vector(InputIterator first, InputIterator last,
                          const allocator_type& alloc)
    : allocator_(alloc),
      size_(std::distance(first, last)),
      capacity_(size_),
      ptr_pool_(std::allocator_traits<A>::allocate(allocator_, capacity_)),
      ptr_data_(ptr_pool_)
{
    this->validate(iterator(ptr_data_), first, last);
}

template <typename T, typename A>
kkk::Vector<T, A>::Vector(std::initializer_list<T> init, const A& alloc) : Vector(init.begin(), init.end(), alloc) {}

template <typename T, typename A>
kkk::Vector<T, A>::Vector(const Vector<T, A>& other)
    : allocator_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())),
      size_(other.size_),
      capacity_(other.capacity_),
      ptr_pool_(std::allocator_traits<A>::allocate(allocator_, capacity_)),
      ptr_data_(ptr_pool_)
{
    this->validate(this->begin(), other.begin(), other.end());
}

template <typename T, typename A>
kkk::Vector<T, A>::Vector(const Vector<T, A>& other,
                          const A& alloc)
    : allocator_(alloc),
      size_(other.size_),
      capacity_(other.capacity_),
      ptr_pool_(std::allocator_traits<A>::allocate(allocator_, capacity_)),
      ptr_data_(ptr_pool_)
{
    this->validate(this->begin(), other.begin(), other.end());
}

template <typename T, typename A>
kkk::Vector<T, A>::Vector(Vector<T, A>&& other) noexcept
    : allocator_(std::move(other.allocator_)),
        size_(std::move(other.size_)),
        capacity_(std::move(other.capacity_)),
        ptr_pool_(std::move(other.ptr_pool_)),
        ptr_data_(std::move(other.ptr_data_))
{
    other.size_ = 0;
    other.capacity_ = 0;
    other.ptr_pool_ = nullptr;
};

template <typename T, typename A>
kkk::Vector<T, A>::Vector(Vector<T, A>&& other, const A& alloc) noexcept
    : allocator_(std::move(alloc)),
        size_(std::move(other.size_)),
        capacity_(std::move(other.capacity_)),
        ptr_pool_(std::move(other.ptr_pool_)),
        ptr_data_(std::move(other.ptr_data_))
{
    other.size_ = 0;
    other.capacity_ = 0;
    other.ptr_pool_ = nullptr;
};

template <typename T, typename A>
kkk::Vector<T, A>::~Vector() {
    this->invalidate_all();
    std::allocator_traits<A>::deallocate(allocator_, ptr_pool_, capacity_);
}



template <typename T, typename A>
kkk::Vector<T, A>& kkk::Vector<T, A>::operator=(Vector<T, A> other) {
    this->swap(other);
    return *this;
}

template <typename T, typename A>
kkk::Vector<T, A>& kkk::Vector<T, A>::operator=(Vector<T, A>&& other) noexcept {
    this->swap(other);
    return *this;
}

template <typename T, typename A>
kkk::Vector<T, A>& kkk::Vector<T, A>::operator=(std::initializer_list<T> init) noexcept {
    Vector tmp = Vector(init);
    this->swap(tmp);
    return *this;
}

template <typename T, typename A>
void kkk::Vector<T, A>::assign(size_type n, const T& value) {
    this->invalidate_all();
    this->reallocation(n);
    this->validate(this->begin(), n, value);
    size_ = n;
}

template <typename T, typename A>
template <typename InputIterator, typename enable_if>
void kkk::Vector<T, A>::assign(InputIterator first, InputIterator last) {
    this->invalidate_all();
    this->reallocation(std::distance(first, last));
    this->validate(this->begin(), first, last);
    size_ = std::distance(first, last);
}

template <typename T, typename A>
void kkk::Vector<T, A>::assign(std::initializer_list <T> init) {
    this->assign(init.begin(), init.end());
}

template <typename T, typename A>
typename kkk::Vector<T, A>::allocator_type kkk::Vector<T, A>::get_allocator() const noexcept {
    return allocator_;
}

template <typename T, typename A>
typename kkk::Vector<T, A>::reference kkk::Vector<T, A>::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("Выход за границы массива");
    }
    return ptr_data_[pos];
}

template <typename T, typename A>
typename kkk::Vector<T, A>::reference kkk::Vector<T, A>::operator[](size_type pos) {
    return ptr_data_[pos];
}

template <typename T, typename A>
typename kkk::Vector<T, A>::const_reference kkk::Vector<T, A>::operator[](size_type pos) const {
    return ptr_data_[pos];
}

template <typename T, typename A>
typename kkk::Vector<T, A>::reference kkk::Vector<T, A>::front() {
    return ptr_data_[0];
}

template <typename T, typename A>
typename kkk::Vector<T, A>::const_reference kkk::Vector<T, A>::front() const {
    return ptr_data_[0];
}


template <typename T, typename A>
typename kkk::Vector<T, A>::reference kkk::Vector<T, A>::back() {
    return ptr_data_[size_ - 1];
}

template <typename T, typename A>
typename kkk::Vector<T, A>::const_reference kkk::Vector<T, A>::back() const {
    return ptr_data_[size_ - 1];
}

template <typename T, typename A>
bool kkk::Vector<T, A>::empty() const noexcept {
    return size_ == 0;
}

template <typename T, typename A>
typename kkk::Vector<T, A>::size_type kkk::Vector<T, A>::size() const noexcept {
    return size_;
}

template <typename T, typename A>
typename kkk::Vector<T, A>::size_type kkk::Vector<T, A>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(T);
}

template <typename T, typename A>
void kkk::Vector<T, A>::reserve(size_type new_capacity) {
    if (new_capacity > this->max_size()) {
        throw std::length_error("Превышен предел выделения памяти");
    }
    if (new_capacity <= capacity_){
        return;
    }

    pointer new_ptr_pool = std::allocator_traits<A>::allocate(allocator_, new_capacity);
    pointer new_ptr_data = new_ptr_pool;
    this->validate(iterator(new_ptr_data), this->begin(), this->end());

    this->invalidate_all();
    std::allocator_traits<A>::deallocate(allocator_, ptr_pool_, capacity_);

    ptr_pool_ = new_ptr_pool;
    ptr_data_ = new_ptr_data;
    capacity_ = new_capacity;
}

template <typename T, typename A>
typename kkk::Vector<T, A>::const_reference kkk::Vector<T, A>::at(kkk::Vector<T, A>::size_type pos) const {
    if (pos >= size_) {
        throw std::out_of_range("Выход за границы массива");
    }
    return ptr_data_[pos];
}

template <typename T, typename A>
typename kkk::Vector<T, A>::size_type kkk::Vector<T, A>::capacity() const noexcept {
    return capacity_;
}

template <typename T, typename A>
void kkk::Vector<T, A>::shrink_to_fit() {
    size_type extra_length = capacity_ - size_;

    if (extra_length == 0) {
        return;
    }

    size_type new_capacity = size_;
    pointer new_ptr_pool = std::allocator_traits<A>::allocate(allocator_, new_capacity);
    pointer new_ptr_data = new_ptr_pool;
    std::move(this->begin(), this->end(), new_ptr_data);

    std::allocator_traits<A>::deallocate(allocator_, ptr_pool_, capacity_);
    ptr_pool_ = new_ptr_pool;
    ptr_data_ = new_ptr_data;
    capacity_ = new_capacity;
}

template <typename T, typename A>
void kkk::Vector<T, A>::clear() noexcept {
    this->invalidate_all();
    size_ = 0;
}

template <typename T, typename A>
void kkk::Vector<T, A>::push_back(const T& value) {
    this->emplace_back(std::forward<const T&>(value));
}

template <typename T, typename A>
void kkk::Vector<T, A>::push_back(T&& x) {
    return this->emplace_back(std::forward<T&&>(x));
}


template <typename T, typename A>
void kkk::Vector<T, A>::pop_back() {
    if (this->empty()) {
        return;
    }
    this->back().~value_type();
    --size_;
}

template <typename T, typename A>
template <typename ...Args>
void kkk::Vector<T, A>::emplace_back(Args&& ... args) {
    this->reallocation(size_ + 1);
    new (ptr_data_ + size_) T(std::forward<Args>(args)...);
    ++size_;
}


template <typename T, typename A>
template <typename ...Args>
typename kkk::Vector<T, A>::iterator kkk::Vector<T, A>::emplace(const_iterator position, Args&& ... args) {
    difference_type index_position = std::distance(this->cbegin(), position);
    this->reallocation(size_ + 1);

    if (index_position <= size_ - 1) {
        this->shift_right(1, iterator(ptr_data_ + index_position), this->end());
    }

    new (ptr_data_ + index_position) T(std::forward<Args>(args)...);
    ++size_;

    return iterator(ptr_data_ + index_position);
}

template <typename T, typename A>
typename kkk::Vector<T, A>::iterator kkk::Vector<T, A>::insert(const_iterator position, const T& x) {
    return this->emplace(position, std::forward<const T&>(x));
}

template <typename T, typename A>
typename kkk::Vector<T, A>::iterator kkk::Vector<T, A>::insert(const_iterator position, size_type n, const T& x) {
    difference_type index_position = std::distance(this->cbegin(), position);
    this->reallocation(size_ + n);

    iterator valid_position = iterator(ptr_data_ + index_position);
    this->shift_right(n, valid_position, this->end());
    this->validate(valid_position, n, x);
    size_ += n;

    return valid_position;
}


template <typename T, typename A>
template <typename InputIterator, typename enable_if>
typename kkk::Vector<T, A>::iterator kkk::Vector<T, A>::insert(const_iterator position, InputIterator first,
                                                         InputIterator last)
{
    difference_type index_position = std::distance(this->cbegin(), position);
    difference_type n = std::distance(first, last);
    this->reallocation(size_ + n);
    iterator valid_position = iterator(ptr_data_ + index_position);
    this->shift_right(n, valid_position, this->end());
    this->validate(valid_position, first, last);
    size_ += n;

    return valid_position;
}

template <typename T, typename A>
typename kkk::Vector<T, A>::iterator kkk::Vector<T, A>::erase(const_iterator position) {
    if (this->empty()) {
        return this->end();
    }
    difference_type index_position = std::distance(this->cbegin(), position);
    auto _position = iterator(ptr_data_ + index_position);
    this->shift_left(1, _position + 1, this->end());
    size_--;
    return _position;
}

template <typename T, typename A>
typename kkk::Vector<T, A>::iterator kkk::Vector<T, A>::erase(const_iterator first, const_iterator last) {
    difference_type index_position = std::distance(this->cbegin(), first);
    size_type n = std::distance(first, last);
    auto _last = iterator(ptr_data_ + std::distance(this->cbegin(), last));
    this->shift_left(n, _last, this->end());
    size_-= n;
    return iterator(ptr_data_ + index_position);
}

template <typename T, typename A>
void kkk::Vector<T, A>::swap(Vector& other) noexcept {
    std::swap(allocator_, other.allocator_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(ptr_pool_, other.ptr_pool_);
    std::swap(ptr_data_, other.ptr_data_);
}

template <typename T, typename A>
void kkk::Vector<T, A>::resize(kkk::Vector<T, A>::size_type n, const value_type& value) {
    if (size_ >= n) {
        this->invalidate(this->begin() + n, this->end());
    } else {
        this->reallocation(n);
        this->validate(this->end(), n - size_, value);
    }
    size_ = n;
}

template <typename T, typename A>
void kkk::Vector<T, A>::resize(kkk::Vector<T, A>::size_type n) {
    if (size_ >= n) {
        this->invalidate(this->begin() + n, this->end());
    } else {
        this->reallocation(n);
        this->validate(this->end(), n - size_);
    }
    size_ = n;
}

template <typename T, typename A>
typename kkk::Vector<T, A>::iterator kkk::Vector<T, A>::insert(kkk::Vector<T, A>::const_iterator position, T&& x) {
    return this->emplace(position, std::forward<T&&>(x));
}
