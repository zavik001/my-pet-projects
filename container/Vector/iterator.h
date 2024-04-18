#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <cstddef>
#include <iterator>

#include "vector.h"
#include "utility.h"
namespace container {
template <typename L>
class RandomAccessIterator {
 public:
  typedef typename iterator_traits<L*>::value_type value_type;
  typedef typename iterator_traits<L*>::pointer pointer;
  typedef typename iterator_traits<L*>::reference reference;
  typedef typename iterator_traits<L*>::difference_type difference_type;
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
      const RandomAccessIterator<typename remove_const<value_type>::type>&
          other)
      : _pointer(&(*other)){

        };

  RandomAccessIterator<value_type>& operator=(
      const RandomAccessIterator<typename remove_const<value_type>::type>&
          other) {
    _pointer = &(*other);
    return (*this);
  }

  RandomAccessIterator& operator++() {
    _pointer++;
    return (*this);
  }

  RandomAccessIterator operator++(int) {
    RandomAccessIterator tmp = *this;
    ++_pointer;
    return tmp;
  }

  RandomAccessIterator& operator--() {
    _pointer--;
    return (*this);
  }

  RandomAccessIterator operator--(int) {
    RandomAccessIterator tmp = *this;
    _pointer--;
    return tmp;
  }

  RandomAccessIterator operator+(const difference_type& other) const {
    return _pointer + other;
  }

  RandomAccessIterator operator-(const difference_type& other) const {
    return _pointer - other;
  }

  RandomAccessIterator& operator+=(const difference_type& other) {
    _pointer = _pointer + other;
    return (*this);
  }

  RandomAccessIterator& operator-=(const difference_type& other) {
    _pointer = _pointer - other;
    return *this;
  }

  pointer operator->() const { return _pointer; }

  reference operator*() const { return *_pointer; }

  reference operator[](difference_type i) const { return *(_pointer + i); }
};

};      // namespace container
#endif  // ITERATOR_H_