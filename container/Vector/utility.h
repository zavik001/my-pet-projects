#ifndef VECTOR_UTILITY_H_
#define VECTOR_UTILITY_H_
#include <iostream>
namespace s21 {
template <class T>
struct remove_const {
  typedef T type;
};

template <class T>
struct remove_const<const T> {
  typedef T type;
};
template <class Iterator>
class iterator_traits {
 public:
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};
template <class T>
class iterator_traits<T*> {
 public:
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef std::random_access_iterator_tag iterator_category;
};
}  // namespace s21

#endif  // VECTOR_UTILITY_H_