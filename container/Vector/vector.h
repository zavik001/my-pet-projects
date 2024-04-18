#ifndef VECTOR_H_
#define VECTOR_H_
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "utility.h"

namespace container {
template <class T, class Alloc = std::allocator<T> >
class Vector {
  template <typename L>
  class RandomAccessIterator {
   public:
    typedef typename iterator_traits<L *>::value_type value_type;
    typedef typename iterator_traits<L *>::pointer pointer;
    typedef typename iterator_traits<L *>::reference reference;
    typedef typename iterator_traits<L *>::difference_type difference_type;
    typedef pointer iterator_type;
    typedef std::random_access_iterator_tag iterator_category;

   private:
    pointer _pointer;

   public:
    RandomAccessIterator()
        : _pointer(){

          };

    RandomAccessIterator(pointer other)
        : _pointer(other){

          };

    virtual ~RandomAccessIterator(){

    };

    RandomAccessIterator(
        const RandomAccessIterator<typename remove_const<value_type>::type>
            &other)
        : _pointer(&(*other)){

          };

    RandomAccessIterator<value_type> &operator=(
        const RandomAccessIterator<typename remove_const<value_type>::type>
            &other) {
      _pointer = &(*other);
      return (*this);
    }

    RandomAccessIterator &operator++() {
      _pointer++;
      return (*this);
    }

    RandomAccessIterator operator++(int) {
      RandomAccessIterator tmp = *this;
      ++_pointer;
      return tmp;
    }

    RandomAccessIterator &operator--() {
      _pointer--;
      return (*this);
    }

    RandomAccessIterator operator--(int) {
      RandomAccessIterator tmp = *this;
      --_pointer;
      return tmp;
    }

    RandomAccessIterator operator+(const difference_type &other) const {
      return _pointer + other;
    }

    RandomAccessIterator operator-(const difference_type &other) const {
      return _pointer - other;
    }

    RandomAccessIterator &operator+=(const difference_type &other) {
      _pointer = _pointer + other;
      return (*this);
    }

    RandomAccessIterator &operator-=(const difference_type &other) {
      _pointer = _pointer - other;
      return *this;
    }

    pointer operator->() const { return _pointer; }

    pointer base() const { return _pointer; }

    reference operator*() const { return *_pointer; }

    reference operator[](difference_type i) const { return *(_pointer + i); }
  };

 public:
  template <typename First, typename Second>
  friend bool operator==(const Vector::RandomAccessIterator<First> &lhs,
                         const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) == &(*rhs);
  }
  template <typename First, typename Second>
  friend bool operator!=(const Vector::RandomAccessIterator<First> &lhs,
                         const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) != &(*rhs);
  }
  template <typename First, typename Second>
  friend bool operator<(const Vector::RandomAccessIterator<First> &lhs,
                        const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) < &(*rhs);
  }
  template <typename First, typename Second>
  friend bool operator>(const Vector::RandomAccessIterator<First> &lhs,
                        const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) > &(*rhs);
  }
  template <typename First, typename Second>
  friend bool operator<=(const Vector::RandomAccessIterator<First> &lhs,
                         const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) <= &(*rhs);
  }
  template <typename First, typename Second>
  friend bool operator>=(const Vector::RandomAccessIterator<First> &lhs,
                         const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) >= &(*rhs);
  }
  template <typename First, typename Second>
  friend typename Vector::RandomAccessIterator<First>::difference_type
  operator-(const Vector::RandomAccessIterator<First> &lhs,
            const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) - &(*rhs);
  }
  template <typename First, typename Second>
  friend typename Vector::RandomAccessIterator<First>::difference_type
  operator+(const Vector::RandomAccessIterator<First> &lhs,
            const Vector::RandomAccessIterator<Second> &rhs) {
    return &(*lhs) + &(*rhs);
  }
  template <typename G>
  friend Vector::RandomAccessIterator<G> operator+(
      const typename Vector::template RandomAccessIterator<G>::difference_type
          &lhs,
      const typename Vector::template RandomAccessIterator<G> &rhs) {
    return lhs + rhs;
  }
  template <typename G>
  friend Vector::RandomAccessIterator<G> operator-(
      const typename Vector::template RandomAccessIterator<T>::difference_type
          &lhs,
      const typename Vector::template RandomAccessIterator<G> &rhs) {
    return lhs - rhs;
  }

 public:
  typedef T value_type;
  typedef Alloc allocator_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::const_pointer const_pointer;
  typedef RandomAccessIterator<value_type> iterator;
  typedef RandomAccessIterator<const value_type> const_iterator;

 private:
  pointer first_;  // указатель на первый элемент
  size_type size_;
  size_type capacity_;        //размер и емкость
  allocator_type allocator_;  //обьект для выделения памяти

 public:
  // default constructor w/o args
  explicit Vector(const allocator_type &alloc = allocator_type())
      : first_(nullptr),
        size_(0),
        capacity_(0),
        allocator_(alloc){
        };
  // добавить конструктор вектора с помощью двух итераторов которые указывают на
  // начало и конец вектора(любой элемент)
  // fill our vector
  explicit Vector(size_type n, const value_type &val = value_type(),
                  const allocator_type &alloc = allocator_type())
      : size_(n), capacity_(n), allocator_(alloc) {
    first_ = allocator_.allocate(n);
    for (size_type i = 0; i < n; i++)
      allocator_.construct(
          first_ + i,
          val);  //заполняем каждый элемент вектора дефолтным значением.(0, "")
  };

  Vector(std::initializer_list<value_type> const &items)
      : Vector(items.size()) {
    std::copy(items.begin(), items.end(), first_);
  }

  Vector(const Vector &other) : Vector(other.size_) {
    std::copy(other.first_, other.first_ + other.size_, first_);
  }

  Vector(Vector &&other)
      : first_(other.first_), size_(other.size_), capacity_(other.capacity_) {
    other.first_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ~Vector() {
    for (size_type i = 0; i < size_; i++) {
      allocator_.destroy(first_ + i);
    }
    if (capacity_ != 0) {
      allocator_.deallocate(first_, capacity_);
    }
  }

  Vector &operator=(const Vector &other) {
    if (this == &other) return (*this);
    for (size_t i = 0; i < this->size_; i++) allocator_.destroy(first_ + i);
    this->size_ = other.size_;
    if (this->capacity_ < this->size_) {
      if (this->capacity_ != 0) this->allocator_.deallocate(first_, capacity_);
      this->capacity_ = this->size_;
      this->first_ = this->allocator_.allocate(this->capacity_);
    }
    for (size_type i = 0; i < this->size_; i++)
      this->allocator_.construct(first_ + i, other[i]);
    return (*this);
  }

  Vector &operator=(Vector &&other) {
    for (size_type i = 0; i < size_; i++) {
      allocator_.destroy(first_ + i);
    }
    if (capacity_ != 0) {
      allocator_.deallocate(first_, capacity_);
    }
    first_ = other.first_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.first_ = nullptr;
    return *this;
  }

  reference at(size_type i) {
    if (i > capacity_) {
      throw std::out_of_range("Out of range");
    }
    return (*(first_ + i));
  }

  const_reference at(size_type i) const {
    if (i > capacity_) {
      throw std::out_of_range("Out of range");
    }
    return (*(first_ + i));
  }

  reference operator[](size_type i) { return (*(first_ + i)); }
  const_reference operator[](size_type i) const { return (*(first_ + i)); }

  reference front() { return (*first_); }
  const_reference front() const { return (*first_); }

  reference back() { return (*(first_ + size_ - 1)); }
  const_reference back() const { return (*first_ + size_ - 1); }

  pointer data() {
    if (size_ != 0) {
      return first_;
    }
    return first_;
  }

  iterator begin() { return iterator(first_); }
  const_iterator begin() const { return const_iterator(first_); }

  iterator end() { return iterator(first_ + size_); }
  const_iterator end() const { return const_iterator(first_ + size_); }

  bool empty() const { return (this->size_ == 0); }

  size_type size() const { return (this->size_); }

  size_type max_size() const { return (this->allocator_.max_size()); }

  size_type capacity() const { return capacity_; }

  void reserve(size_type reserve_size) {
    if (reserve_size > max_size()) {
      throw std::length_error("vector::reserve");
    } else if (reserve_size >= size_) {
      value_type *newarr = allocator_.allocate(reserve_size);
      for (size_type i = 0; i < size_; ++i) {
        newarr[i] = std::move(first_[i]);
        allocator_.destroy(first_ + i);
      }
      allocator_.deallocate(first_, capacity_);
      first_ = newarr;
      capacity_ = reserve_size;
    }
  }

  void shrink_to_fit() {
    if (size_ != capacity_) reserve(size_);
  }

  void clear() {
    for (size_type i = 0; i < size_; ++i) {
      allocator_.destroy(first_ + i);
    }
    size_ = 0;
  }

  iterator insert(iterator position, const value_type &val) {
    if (position < begin() || position > end())
      throw std::logic_error("Vector");
    difference_type distance = position - begin();
    if (size_ == capacity_) {
      if (capacity_ == 0) {
        capacity_ = 1;
      } else {
        capacity_ = capacity_ * 2;
      }
      pointer newArr = allocator_.allocate(capacity_);
      std::uninitialized_copy(begin(), position, iterator(newArr));
      allocator_.construct(newArr + distance, val);
      std::uninitialized_copy(position, end(), iterator(newArr + distance + 1));
      for (size_t i = 0; i < size_; i++) {
        allocator_.destroy(first_ + i);
      }
      allocator_.deallocate(first_, size_);
      size_++;
      first_ = newArr;
    } else {
      for (size_type i = size_ - 1; i > static_cast<size_type>(distance); i--) {
        allocator_.destroy(first_ + i);
        allocator_.construct(first_ + i, *(first_ + i - 1));
      }
      allocator_.destroy(&(*position));
      allocator_.construct(&(*position), val);
      size_++;
    }
    return (begin() + distance);
  }

  void erase(iterator iter) {
    auto it_begin = begin();
    size_type pos = iter.base() - it_begin.base();
    if (pos >= capacity_) {
      throw std::out_of_range("Out of range");
    }
    for (size_type i = pos; i <= size() - 1; i++) {
      first_[i] = first_[i + 1];
    }
    size_--;
    allocator_.destroy(iter.base());
  }

  void push_back(const value_type &value) {
    if (size_ == capacity_) {
      if (capacity_ == 0)
        reserve(1);
      else
        reserve(capacity_ * 2);
    }
    allocator_.construct(first_ + size_, value);
    size_++;
  }

  void pop_back() noexcept {
    if (size_ > 0) {
      allocator_.destroy(first_ + size_ - 1);
      size_--;
    }
  }

  void swap(Vector &x) {
    if (this != &x) {
      std::swap(first_, x.first_);
      std::swap(size_, x.size_);
      std::swap(capacity_, x.capacity_);
      std::swap(allocator_, x.allocator_);
    }
  };

  void resize(size_type new_size, const value_type &val = value_type()) {
    if (new_size > size_) {
      if (size_ <= capacity_) reserve(new_size);
      while (new_size > size_) {
        allocator_.construct(first_ + size_++, val);
      }
    }
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&...args) {
    iterator iterator(const_cast<pointer>(pos.base()));
    return insert(iterator, value_type(std::forward<Args>(args)...));
  }

  template <class... Args>
  void emplace_back(Args &&...args) {
    push_back(value_type(std::forward<Args>(args)...));
  }
};
}  // namespace container
#endif  // VECTOR_H_