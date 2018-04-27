//
// Created by oem on 24.04.18.
//

#include <cstring>
#include "vector.h"

template <typename T>
kkk::Vector<T>::Vector()
    : allocator_(),
      ptr_pool_(nullptr),
      size_(0),
      capacity_(0)
{}

template <typename T>
kkk::Vector<T>::Vector(size_t n)
    : allocator_(),
      ptr_pool_(nullptr),
      size_(n),
      capacity_(n)
{
    ptr_pool_ = allocator_.allocate(n);
    value_type td;
    for (size_type i = 0; i < n; i++) {
        new (ptr_pool_ + i) value_type();
        td = *(ptr_pool_ + i);
    }
}

template <typename T>
kkk::Vector<T>::Vector(size_t n, const T& value)
    : allocator_(),
      ptr_pool_(nullptr),
      size_(n),
      capacity_(n)
{
    ptr_pool_ = allocator_.allocate(n);
    for (size_type i = 0; i < n; i++) {
        new (ptr_pool_ + i) value_type(value);
    }
}

template <typename T>
kkk::Vector<T>::Vector(const Vector<T>& other)
    : allocator_(other.allocator_),
      ptr_pool_(nullptr),
      size_(other.size_),
      capacity_(other.capacity_)
{
    ptr_pool_ = allocator_.allocate(capacity_);
    std::memcpy(ptr_pool_, other.ptr_pool_, size_ * sizeof(value_type));
}

template <typename T>
kkk::Vector<T>::Vector(Vector<T>&& other)
    : allocator_(std::move(other.allocator_)),
      ptr_pool_(std::move(other.ptr_pool_)),
      size_(std::move(other.size_)),
      capacity_(std::move(other.capacity_))
{
    other.size_ = 0;
    other.capacity_ = 0;
    other.ptr_pool_ = nullptr;
};


template <typename T>
kkk::Vector<T>::~Vector() {
    for (size_type i = 0; i < size_; i++) {
        ptr_pool_[i].~value_type();
    }
    allocator_.deallocate(ptr_pool_, capacity_);
}

template <typename T>
T*kkk::Vector<T>::data() noexcept {
    if (size_) {
        return ptr_pool_;
    }
    return nullptr;
}

template <typename T>
const T*kkk::Vector<T>::data() const noexcept {
    if (size_) {
        return ptr_pool_;
    }
    return nullptr;
}


template class kkk::Vector<int>;
template class kkk::Vector<double>;