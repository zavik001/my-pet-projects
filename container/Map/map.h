#ifndef MAP_H_
#define MAP_H_
#include <memory>

#include "../Set/compare.h"
#include "../Vector/vector.h"
#include "../Vector/utility.h"
#include "red_black_tree.h"
namespace container {

template <class Key, class T, class Compare = container::Pair_compare<Key, T>,
          class Allocator = std::allocator<std::pair<const Key, T>>>

class Map {
 public:
  typedef Key key_type;
  typedef T mapped_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Compare key_compare;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef Allocator allocator_type;
  typedef const value_type& reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef container::RedBlackTree<value_type, key_compare, allocator_type> tree_type;
  typedef typename tree_type::iterator iterator;
  typedef typename tree_type::const_iterator const_iterator;

 private:
  allocator_type alloc_;
  tree_type tree_;
  key_compare compare_;

 public:
  explicit Map(const key_compare& comp = key_compare(),
               const allocator_type& alloc = allocator_type())
      : alloc_(alloc), tree_(tree_type(comp, alloc)), compare_(comp) {}

  Map(std::initializer_list<value_type> const& items) {
    for (auto it = items.begin(); it != items.end(); it++)
        insert(*it);
  }
  Map(Map& other) : Map() { copy(other); }
  Map(Map&& other) noexcept : Map() {
    copy(other);
    other.clear();
  }
  ~Map() { tree_.clear(); }
  Map& operator=(const Map& other) {
    copy(other);
    return *this;
  }

  void copy(const Map& other) {
    clear();
    for (auto it = other.begin(); it != other.end(); it++)
        insert(*it);
  }

  mapped_type& operator[](const key_type& key) {
    iterator tmp = this->find(key);
			if (tmp == this->end())
				insert(std::make_pair(key, mapped_type()));
			tmp = this->find(key);
			return ((*tmp).second);
  }

  size_type size() const { return (tree_.size()); }

  size_type max_size() const { return (tree_.max_size()); }

  bool empty() const { return (tree_.empty()); }

  const_iterator begin() const { return tree_.begin(); }

  const_iterator end() const { return tree_.end(); }
  iterator begin() { return tree_.begin(); }

  iterator end() { return tree_.end(); }
  void clear() { tree_.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) {
    return (tree_.insert(value));
  }

  iterator insert(iterator hint, const value_type& value) {
    return (tree_.insert(hint, value));
  }

  void erase(iterator pos) { tree_.erase(pos); }

  size_type erase(const Key& key) {
    return (tree_.erase(std::make_pair(key, mapped_type())));
  }

  void erase(iterator first, iterator last) { tree_.erase(first, last); }

  iterator find(const Key& key) {
    return tree_.find(std::make_pair(key, mapped_type()));
  }

  size_type count(const Key& key) const {
    return (tree_.count(std::make_pair(key, mapped_type())));
  }

  void swap(Map& other) {
    std::swap(this->compare_, other.compare_);
    tree_.swap(other.tree_);
  }

  iterator lower_bound(const key_type& key) {
    return (tree_.lower_bound(std::make_pair(key, mapped_type())));
  }

  const_iterator lower_bound(const key_type& key) const {
    return (tree_.lower_bound(std::make_pair(key, mapped_type())));
  }

  iterator upper_bound(const key_type& key) {
    return (tree_.upper_bound(std::make_pair(key, mapped_type())));
  }

  const_iterator upper_bound(const key_type& key) const {
    return (tree_.upper_bound(std::make_pair(key, mapped_type())));
  }

  std::pair<iterator, iterator> equal_range(const key_type& key) {
    return (tree_.equal_range(std::make_pair(key, mapped_type())));
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    return (tree_.equal_range(std::make_pair(key, mapped_type())));
  }

  mapped_type& at(const key_type& key) {
    iterator tmp = this->find(key);
//			if (tmp == this->end()) {
//				throw std::out_of_range("ups");
//      }
			return ((*tmp).second);
  }
  
  const mapped_type& at(const key_type& key) const {
      iterator tmp = this->find(key);
//      if (tmp == this->end()) {
//          throw std::out_of_range("ups");
//      }
      return ((*tmp).second);
  }

  void merge(Map& other) {
      for (auto it = other.begin(); it != other.end(); it++) {
          this->insert(*it);
      }
      other.clear();
  }

  std::pair<iterator, bool> insert_or_assign(const key_type& key,
                                             const mapped_type& obj) {
      size_type start_size = this->size();
      if (contains(key)) {
          this->at(key) = obj;
      } else {
          this->insert(std::make_pair(key, obj));
      }
          return std::make_pair(find(key), start_size < this->size());
  }

  template <class... Args>
  Vector<std::pair<iterator, bool>> emplace(Args&&... args) {
    Vector<std::pair<iterator, bool>> result;
    result.push_back(insert(std::forward<Args>(args)...));
    return result;
  }
  bool contains(const Key& key) {
      bool res = false;
      if(find(key).node() != nullptr) {
          res = true;
      }
      return res;
  }

  template <class _Key, class _T, class _Compare, class _Alloc>
  friend bool operator==(const Map<_Key, _T, _Compare, _Alloc>& lhs,
                         const Map<_Key, _T, _Compare, _Alloc>& rhs);

  template <class _Key, class _T, class _Compare, class _Alloc>
  friend bool operator<(const Map<_Key, _T, _Compare, _Alloc>& lhs,
                        const Map<_Key, _T, _Compare, _Alloc>& rhs);
};

template <class Key, class T, class Compare, class Alloc>
bool operator==(const Map<Key, T, Compare, Alloc>& lhs,
                const Map<Key, T, Compare, Alloc>& rhs) {
  return (lhs.tree_ == rhs.tree_);
}

template <class Key, class T, class Compare, class Alloc>
bool operator!=(const Map<Key, T, Compare, Alloc>& lhs,
                const Map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator<(const Map<Key, T, Compare, Alloc>& lhs,
               const Map<Key, T, Compare, Alloc>& rhs) {
  return (lhs.tree_ < rhs.tree_);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>(const Map<Key, T, Compare, Alloc>& lhs,
               const Map<Key, T, Compare, Alloc>& rhs) {
  return (rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator<=(const Map<Key, T, Compare, Alloc>& lhs,
                const Map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs > rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>=(const Map<Key, T, Compare, Alloc>& lhs,
                const Map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

template <class Key, class T, class Compare, class Alloc>
void swap(const Map<Key, T, Compare, Alloc>& lhs,
          const Map<Key, T, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}
}  // namespace container
#endif  // MAP_H_