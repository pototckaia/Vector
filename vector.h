//
// Created by oem on 24.04.18.
//
#pragma once

#include <iterator>

namespace kkk
{

template <typename T> class Vector;

//template <typename T>
//    bool operator==(const Vector<T>&, const Vector<T>&);
//template <typename T>
//    bool operator< (const Vector<T>&, const Vector<T>&);
//template <typename T>
//    bool operator!=(const Vector<T>&, const Vector<T>&);
//template <typename T>
//    bool operator> (const Vector<T>&, const Vector<T>&);
//template <typename T>
//    bool operator>=(const Vector<T>&, const Vector<T>&);
//template <typename T>
//    bool operator<=(const Vector<T>&, const Vector<T>&);
//
//template <typename T>
//    void swap(Vector<T>&, Vector<T>&);

template <typename T>
class Vector {
public:
    class VectorIterator;

    // types:
    typedef T& reference;
    typedef const T& const_reference;

    typedef VectorIterator iterator;
//    typedef const T* const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
//    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    typedef T value_type;

    typedef std::allocator<T> allocator_type;

    typedef typename std::allocator_traits<allocator_type>::pointer pointer;
    typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;

    // iterator:
    class VectorIterator { /// how do const_iterator
    public:
        VectorIterator() : ptr_data_(nullptr) {}
        VectorIterator(const VectorIterator& other) : ptr_data_(other.ptr_data_) {}
        ~VectorIterator() {}
        VectorIterator& operator=(const VectorIterator& other) {
            ptr_data_ = other.ptr_data_;
            return *this;
        }

        VectorIterator& operator++() {
            ptr_data_++;
            return *this;
        }
        VectorIterator& operator--() {
            ptr_data_--;
            return *this;
        }
        // VectorIterator operator++(int) {} ?
        // VectorIterator& operator+=(size_type)
        // VectorIterator operator+(size_type) const
        // friend VectorInterator operator+(size_type,const VectorIterator&)
        // difference_type operator-(VectorIterator) const

        reference operator*() const {
            return *ptr_data_;
        }
        pointer operator->() const {
            return ptr_data_;
        }

        bool operator==(const VectorIterator& other) const {
            return ptr_data_ == other.ptr_data_;
        }
        bool operator!=(const VectorIterator& other) const {
            return ptr_data_ != other.ptr_data_;
        }

    private:
        pointer ptr_data_;
    };
    
    // construct/copy/destroy:
    explicit Vector();
    explicit Vector(size_type);
    Vector(size_type, const T&);

    // template <typename InputIterator>
    // Vector(InputIterator, InputIterator);

     Vector(const Vector<T>&);
     Vector(Vector&&);
    // // Vector(const Vector&, const Allocator&);
    // // Vector(Vector&&, const Allocator&);
    // Vector(std::initializer_list<T>);

    ~Vector();

    // Vector<T>& operator=(const vector<T>&);
    // Vector<T>& operator=(vector<T>&&);
    // Vector& operator=(initializer_list<T>);

    // template <class InputIterator>
    // void assign(InputIterator, InputIterator);

    // void assign(size_type, const T&);
    // void assign(initializer_list<T>);

     allocator_type get_allocator() const noexcept { return allocator_; }

    // // iterators:
    // iterator begin() noexcept;
    // const_iterator begin() const noexcept;

    // iterator end() noexcept;
    // const_iterator end() const noexcept;

    // reverse_iterator rbegin() noexcept;
    // const_reverse_iterator rbegin() const noexcept;

    // reverse_iterator rend() noexcept;
    // const_reverse_iterator rend() const noexcept;

    // const_iterator cbegin() noexcept;
    // const_iterator cend() noexcept;

    // const_reverse_iterator crbegin() const noexcept;
    // const_reverse_iterator crend() const noexcept;

    // // capacity:
     size_type size() const noexcept { return size_; }
    // size_type max_size() const noexcept;
    // void resize(size_type);
    // void resize(size_type, const T&);
     size_type capacity() const noexcept { return capacity_; }


     bool empty() const noexcept {return size_ <= 0;}
    // void reserve(size_type);
    // void shrink_to_fit();

    // // element access:
     reference operator[](size_type i) { return ptr_pool_[i]; }
     const_reference operator[](size_type i) const { return ptr_pool_[i]; }

    // reference at(size_type n);
    // const_reference at(size_type n) const;

    // reference front();
    // const_reference front() const;

    // reference back();
    // const_reference back() const;

    // //data access
     T*       data() noexcept;
     const T* data() const noexcept;

    // // modifiers:
    // template <typename... Args> void emplace_back(Args&&... args);
    // void push_back(const T&);
    // void push_back(T&&);
    // void pop_back();

    // template <typename... Args> iterator emplace(const_iterator position, Args&&... args);
    // iterator insert(const_iterator position, const T& x);
    // iterator insert(const_iterator position, T&& x);
    // iterator insert(const_iterator position, size_type n, const T& x);

    // template <class InputIterator>
    // iterator insert (const_iterator position, InputIterator first,
    //                  InputIterator last);
    // iterator insert(const_iterator position, initializer_list<T>);

    // iterator erase(const_iterator position);
    // iterator erase(const_iterator first, const_iterator last);
    // void     swap(vector<T,Allocator>&);
    // void     clear() noexcept;
private:
    pointer ptr_pool_;
    allocator_type allocator_;
    size_type size_;
    size_type capacity_;

};
} // test

