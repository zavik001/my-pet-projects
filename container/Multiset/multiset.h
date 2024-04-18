#ifndef MULTISET_H_
#define MULTISET_H_

#include "../Set/compare.h"
#include "../Set/set.h"
namespace container {
template <class T, class Compare = container::Compare<T>>
class Multiset : public Set<T, Compare> {
 public:
  using key_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using value_type = key_type;
  using compare_ = Compare;
  using node_type = TreeNode<key_type, compare_>;
  using node_pointer = typename Set<value_type, compare_>::node_pointer;
  using iterator = TreeIterator<key_type, compare_>;
  using const_iterator = TreeConstIterator<key_type, compare_>;

 private:
  std::allocator<Set<T, Compare>> alloc_;

 public:
  Multiset() : Set<key_type, compare_>() {}
  Multiset(std::initializer_list<key_type> const &items)
      : Set<key_type, compare_>() {
    for (auto it = items.begin(); it != items.end(); it++) insert(*it);
  }
  Multiset(const Multiset &other) : Set<key_type, compare_>() {
    for (auto it = other.begin(); it != other.end(); it++) insert(*it);
  }
  Multiset(Multiset &&other) : Set<key_type, compare_>() {
    delete this->nill_;
    this->root_ = other.root_;
    this->size_ = other.size_;
    this->nill_ = other.nill_;
    other.root_ = nullptr;
    other.nill_ = nullptr;
    other.size_ = 0;
  }
  ~Multiset() {}

 public:
  size_type max_size() { return alloc_.max_size(); }
  void operator=(Multiset &&other) {
    Set<value_type, compare_>::Clear_tree(this->root_);
    delete this->nill_;
    this->root_ = other.root_;
    this->size_ = other.size_;
    this->nill_ = other.nill_;
    other.root_ = nullptr;
    other.nill_ = nullptr;
    other.size_ = 0;
  }

  iterator insert(const value_type &value) {
    // если root пустой , то вставляем на его место
    // если не пустой, тогда 1: если элемент существует , то добавляем
    // 2: если такого элемента нет, то его закидываем по правилам
    return Set<value_type, compare_>::insert(value).first;
  }

  void merge(Multiset &other) {
    return Set<value_type, compare_>::merge(other);
  }

  size_type count(const value_type &key) {
    size_type result = 0;
    if (this->root_ != nullptr) {
      for (auto it = this->begin(); it != this->end(); it++) {
        if (this->comp_.eq(*it, key)) {
          result++;
        }
      }
    }
    return result;
  }

  std::pair<iterator, iterator> equal_range(const value_type &key) {
    auto it_begin = this->find(key);
    auto it_end = it_begin;
    for (; this->comp_.eq(*it_end, key) && it_end != this->end(); it_end++) {
      ;
    }
    std::pair<iterator, iterator> res_pair = {it_begin, it_end};
    return res_pair;
  }
  iterator lower_bound(const value_type &key) {
    auto result_it = this->begin();
    for (; this->comp_.lt(*result_it, key) && result_it != this->end();
         result_it++) {
      ;
    }
    return result_it;
  }
  iterator upper_bound(const value_type &key) {
    auto result_it = this->begin();
    for (; this->comp_.le(*result_it, key) && result_it != this->end();
         result_it++) {
      if (result_it == this->end()) break;
    }
    return result_it;
  }

  // helpers
 private:
};
}  // namespace container

#endif  //  MULTISET_H_