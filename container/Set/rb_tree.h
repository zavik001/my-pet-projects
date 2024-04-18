#ifndef RB_TREE_H_
#define RB_TREE_H_
#include <iomanip>
#include <iostream>
#include <memory>

#include "../Vector/utility.h"
#include "rb_tree_node.h"
namespace container {
template <class Value, class Compare = std::less<Value>,
          class Alloc = std::allocator<Value> >
class RedBlackTree {
 public:
  using value_type = Value;
  using value_compare = Compare;
  using allocator_type = Alloc;
  using node_allocator = typename Alloc::template rebind<Node<Value> >::other;
  using node_pointer = typename node_allocator::pointer;
  using reference = typename allocator_type::reference;
  using const_reference = typename allocator_type::const_reference;
  using pointer = typename allocator_type::pointer;
  using const_pointer = typename allocator_type::const_pointer;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using iterator = TreeIterator<Value>;
  using const_iterator = TreeIterator<const Value>;

 public:
  allocator_type alloc_;
  node_allocator node_alloc_;
  value_compare compare_;
  node_pointer nil_;
  node_pointer root_;
  node_pointer header_;
  size_type size_;

 public:
  //____________________________Helpers_________________________________//
  bool is_nill(node_pointer elem) { return (elem->right_ == nullptr); }
  node_pointer tree_min(node_pointer n) const {
    while (n != NULL && !is_nil(n->left_)) n = n->left_;
    return n;
  }

  node_pointer tree_max(node_pointer n) const {
    while (n != NULL && !is_nil(n->right_)) {
      n = n->right_;
    }
    return n;
  }

  void _rotate_right(node_pointer node) {
    node_pointer y;

    y = node->left_;
    node->left_ = y->right_;
    if (!is_nil(y->right_)) y->right_->parent_ = node;
    y->parent_ = node->parent_;
    if (node->parent_ == NULL)
      root_ = y;
    else if (node == node->parent_->left_)
      node->parent_->left_ = y;
    else
      node->parent_->right_ = y;
    y->right_ = node;
    node->parent_ = y;
  }

  void _rotate_left(node_pointer node) {
    node_pointer y;

    y = node->right_;
    node->right_ = y->left_;
    if (!is_nil(y->left_)) y->left_->parent_ = node;
    y->parent_ = node->parent_;
    if (node->parent_ == NULL)
      root_ = y;
    else if (node == node->parent_->left_)
      node->parent_->left_ = y;
    else
      node->parent_->right_ = y;
    y->left_ = node;
    node->parent_ = y;
  }

  node_pointer _insert(node_pointer new_node) {
    if (root_ == header_)
      root_ = new_node;
    else
      _insert_to_node(root_, new_node);
    return new_node;
  }

  node_pointer _insert_to_node(node_pointer kekis, node_pointer new_node) {
    if (compare_(*new_node->value_, *kekis->value_)) {
      if (!is_nil(kekis->left_))
        return (_insert_to_node(kekis->left_, new_node));
      kekis->left_ = new_node;
    } else {
      if (!is_nil(kekis->right_))
        return (_insert_to_node(kekis->right_, new_node));
      kekis->right_ = new_node;
    }
    new_node->parent_ = kekis;
    return (new_node);
  }

  node_pointer _insert_into_tree(node_pointer new_node, node_pointer where) {
    if (root_ == header_)
      root_ = new_node;
    else
      _insert_to_node(where, new_node);
    return (new_node);
  }

  void _insert_fixup(node_pointer node) {
    if (node != root_ && node->parent_ != root_) {
      while (node != root_ && !node->parent_->is_black) {
        if (node->parent_ == node->parent_->parent_->left_) {
          node_pointer uncle = node->parent_->parent_->right_;
          if (!uncle->is_black) {
            node->parent_->is_black = true;
            uncle->is_black = true;
            node->parent_->parent_->is_black = false;
            node = node->parent_->parent_;
          } else {
            if (node == node->parent_->right_) {
              node = node->parent_;
              _rotate_left(node);
            }
            node->parent_->is_black = true;
            node->parent_->parent_->is_black = false;
            _rotate_right(node->parent_->parent_);
          }
        } else {
          node_pointer uncle = node->parent_->parent_->left_;
          if (!uncle->is_black) {
            node->parent_->is_black = true;
            uncle->is_black = true;
            node->parent_->parent_->is_black = false;
            node = node->parent_->parent_;
          } else {
            if (node == node->parent_->left_) {
              node = node->parent_;
              _rotate_right(node);
            }
            node->parent_->is_black = true;
            node->parent_->parent_->is_black = false;
            _rotate_left(node->parent_->parent_);
          }
        }
      }
    }
    root_->is_black = true;
  }

  bool is_nil(node_pointer node) const {
    return node == nil_ || node == header_;
  }

  void clear_node(node_pointer node) {
    if (node && !is_nil(node)) {
      clear_node(node->right_);
      clear_node(node->left_);
      alloc_.destroy(node->value_);
      alloc_.deallocate(node->value_, 1);
      node_alloc_.deallocate(node, 1);
    }
  }

  void transplant(node_pointer position, node_pointer node) {
    if (position == root_) {
      root_ = node;
    } else if (position == position->parent_->left_) {
      position->parent_->left_ = node;
    } else
      position->parent_->right_ = node;
    node->parent_ = position->parent_;
  }

  void free_node(node_pointer node) {
    alloc_.destroy(node->value_);
    alloc_.deallocate(node->value_, 1);
    node_alloc_.deallocate(node, 1);
  }

  void init_nil_head() {
    nil_ = node_alloc_.allocate(1);
    node_alloc_.construct(nil_, Node<Value>());
    nil_->is_black = true;
    nil_->is_nil = true;
    header_ = node_alloc_.allocate(1);
    node_alloc_.construct(header_, Node<Value>());
    header_->value_ = alloc_.allocate(1);
    alloc_.construct(header_->value_, Value());
    header_->is_black = true;
  }

  iterator begin() {
    return iterator(size_ == 0 ? header_ : iterator(tree_min(root_)));
  }

  const_iterator begin() const {
    return const_iterator(size_ == 0 ? header_
                                     : const_iterator(tree_min(root_)));
  }

  iterator end() { return iterator(header_); }

  const_iterator end() const { return const_iterator(header_); }

  pointer create_value(const value_type& value) {
    pointer new_val = alloc_.allocate(1);
    alloc_.construct(new_val, value);
    return new_val;
  }

  node_pointer copy_node(node_pointer other) {
    node_pointer new_node = node_alloc_.allocate(1);
    node_alloc_.construct(new_node, Node<Value>());
    new_node->is_black = other->is_black;
    new_node->is_nil = other->is_nil;
    if (other->value_) {
      new_node->value_ = alloc_.allocate(1);
      alloc_.construct(new_node->value_, *other->value_);
    }
    return new_node;
  }

  void copy_child(node_pointer my_node, node_pointer other) {
    if (other->left_->is_nil)
      my_node->left_ = nil_;
    else {
      my_node->left_ = copy_node(other->left_);
      my_node->left_->parent_ = my_node;
      copy_child(my_node->left_, other->left_);
    }
    if (other->right_->is_nil)
      my_node->right_ = nil_;
    else if (other->right_->right_ == NULL) {
      my_node->right_ = header_;
      header_->parent_ = my_node;
    } else {
      my_node->right_ = copy_node(other->right_);
      my_node->right_->parent_ = my_node;
      copy_child(my_node->right_, other->right_);
    }
  }

  node_pointer search(const value_type& value, node_pointer node) const {
    if (!node || is_nil(node)) {
      return NULL;
    }
    if (compare_(value, *(node->value_))) {
      return search(value, node->left_);
    }
    if (compare_(*(node->value_), value)) {
      return search(value, node->right_);
    }
    return node;
  }

  iterator find(const value_type& value) {
    node_pointer res = search(value, root_);
    return res == NULL ? end() : iterator(res);
  }

  const_iterator find(const value_type& value) const {
    node_pointer res = search(value, root_);
    return res == NULL ? end() : const_iterator(res);
  }

  std::pair<iterator, bool> insert(value_type const& value_) {
    node_pointer find_val = search(value_, root_);
    if (find_val) return std::pair<iterator, bool>(iterator(find_val), false);
    node_pointer new_node = node_alloc_.allocate(1);
    node_alloc_.construct(new_node, Node<value_type>(create_value(value_)));
    new_node->left_ = nil_;
    new_node->right_ = nil_;
    _insert_into_tree(new_node, root_);
    std::pair<iterator, bool> res(iterator(new_node), true);
    _insert_fixup(new_node);
    size_++;
    new_node = tree_max(root_);
    new_node->right_ = header_;
    header_->parent_ = new_node;
    return res;
  }

  iterator insert(iterator position, const value_type& value) {
    node_pointer new_node = search(value, root_);
    if (new_node) return (iterator(new_node));
    new_node = node_alloc_.allocate(1);
    node_alloc_.construct(new_node, Node<value_type>(create_value(value)));
    new_node->left_ = nil_;
    new_node->right_ = nil_;
    if (position == begin()) {
      if (position != end() && compare_(value, *position))
        _insert_into_tree(new_node, tree_min(root_));
      else
        _insert_into_tree(new_node, root_);
    } else if (position == end()) {
      if (position != begin() && compare_(*(--position), value))
        _insert_into_tree(new_node, header_->parent_);
      else
        _insert_into_tree(new_node, root_);
    } else
      _insert_into_tree(new_node, root_);
    _insert_fixup(new_node);
    size_++;
    node_pointer max_of_tree = tree_max(root_);
    max_of_tree->right_ = header_;
    header_->parent_ = max_of_tree;
    return (iterator(new_node));
  }

  //   template<class InputIt>
  // void insert(typename ft::enable_if< !ft::is_integral<InputIt>::value,
  // InputIt >::type first, InputIt last) {
  //     for (; first != last; ++first)
  // 	    insert(*first);
  // }

  void erase(iterator pos) {
    node_pointer y = pos.base(), x, for_free = y;
    bool y_original_is_black = y->is_black;
    if (is_nil(y->left_)) {
      x = y->right_;
      transplant(y, y->right_);
    } else if (is_nil(y->right_)) {
      x = y->left_;
      transplant(y, y->left_);
    } else {
      node_pointer z = y;
      y = tree_min(z->right_);
      y_original_is_black = y->is_black;
      x = y->right_;
      if (y->parent_ != z) {
        transplant(y, y->right_);
        y->right_ = z->right_;
        z->right_->parent_ = y;
      }
      transplant(z, y);
      y->left_ = z->left_;
      y->left_->parent_ = y;
      y->is_black = z->is_black;
    }
    free_node(for_free);
    if (y_original_is_black) erase_fixup(x);
    size_--;
    nil_->parent_ = NULL;
    if (size_ == 0)
      root_ = header_;
    else {
      if (size_ != 1)
        x = tree_max(root_);
      else
        x = root_;
      x->right_ = header_;
      header_->parent_ = x;
    }
  }

  size_type erase(const value_type& value) {
    node_pointer res = search(value, root_);
    if (res) erase(iterator(res));
    return (res != NULL);
  }

  void erase(iterator first, iterator last) {
    while (first != last) erase(first++);
  }

  void erase_fixup(node_pointer x) {
    node_pointer brother;
    while (x != root_ && x->is_black) {
      if (x == x->parent_->left_) {
        brother = x->parent_->right_;
        if (!brother->is_black) {
          brother->is_black = true;
          x->parent_->is_black = false;
          _rotate_left(x->parent_);
          brother = x->parent_->right_;
        }
        // case 2
        if (brother->left_->is_black && brother->right_->is_black) {
          brother->is_black = false;
          x = x->parent_;
        } else {
          // case 3
          if (brother->right_->is_black) {
            brother->left_->is_black = true;
            brother->is_black = false;
            _rotate_right(brother);
            brother = x->parent_->right_;
          }
          // case 4
          brother->is_black = x->parent_->is_black;
          x->parent_->is_black = true;
          brother->right_->is_black = true;
          _rotate_left(x->parent_);
          x = root_;
        }
      } else {
        brother = x->parent_->left_;
        // case 1
        if (!brother->is_black) {
          brother->is_black = true;
          x->parent_->is_black = false;
          _rotate_right(x->parent_);
          brother = x->parent_->left_;
        }
        // case 2
        if (brother->right_->is_black && brother->left_->is_black) {
          brother->is_black = false;
          x = x->parent_;
        } else {
          // case 3
          if (brother->left_->is_black) {
            brother->right_->is_black = true;
            brother->is_black = false;
            _rotate_left(brother);
            brother = x->parent_->left_;
          }
          // case 4
          brother->is_black = x->parent_->is_black;
          x->parent_->is_black = true;
          brother->left_->is_black = true;
          _rotate_right(x->parent_);
          x = root_;
        }
      }
    }
  }

  //____________________________Constructors_________________________________//

  RedBlackTree(const Compare& compare,
               const allocator_type& a = allocator_type())
      : alloc_(a),
        node_alloc_(node_allocator()),
        compare_(compare),
        root_(0),
        size_(0) {
    init_nil_head();
    root_ = header_;
  }

  RedBlackTree()
      : root_(0),
        alloc_(allocator_type()),
        node_alloc_(node_allocator()),
        compare_(value_compare()),
        size_(0) {
    init_nil_head();
    root_ = header_;
  }

  RedBlackTree(const RedBlackTree& other)
      : compare_(other.compare_), root_(NULL) {
    *this = other;
  }

  //   template<class InputIt>
  // RedBlackTree(typename ft::enable_if< !ft::is_integral<InputIt>::value,
  // InputIt >::type first, InputIt last, 	const value_compare& comp, const
  // allocator_type& alloc = allocator_type()):
  // _alloc(alloc),node_alloc_(node_allocator()),compare_(comp) {
  // 	init_nil_head();
  // 	root_ = header_;
  // 	for (; first != last; ++first)
  // 		insert(*first);
  // }

  RedBlackTree& operator=(const RedBlackTree& other) {
    if (this == &other) return *this;
    node_alloc_ = other.node_alloc_;
    alloc_ = other.alloc_;
    compare_ = other.compare_;
    if (root_ == NULL)
      init_nil_head();
    else
      clear_node(root_);
    if (other.size_ == 0)
      this->root_ = this->header_;
    else {
      this->root_ = copy_node(other.root_);
      copy_child(this->root_, other.root_);
    }
    this->size_ = other.size_;
    return *this;
  }

  ~RedBlackTree() {
    //    clear_node(root_);
    //    alloc_.destroy(header_->value_);
    //    alloc_.deallocate(header_->value_, 1);
    //    node_alloc_.deallocate(nil_, 1);
    //    node_alloc_.deallocate(header_, 1);
  }
  //_______________________________________________________________________________________________________________//
  size_type size() const { return size_; }

  size_type max_size() const { return alloc_.max_size(); }

  bool empty() const { return size_ == 0; }

  value_compare value_comp() const { return compare_; }

  void clear() {
    clear_node(root_);
    root_ = header_;
    header_->parent_ = NULL;
    size_ = 0;
  }

  size_type count(const value_type& value) const {
    return find(value) != end();
  }

  iterator lower_bound(const value_type& value) {
    iterator last = end();
    for (iterator first = begin(); first != last; ++first) {
      if (!compare_(*first, value)) return first;
    }
    return last;
  }

  const_iterator lower_bound(const value_type& value) const {
    const_iterator last = end();
    for (const_iterator first = begin(); first != last; ++first) {
      if (compare_(*first, value)) return first;
    }
    return last;
  }

  iterator upper_bound(const value_type& value) {
    iterator last = end();
    for (iterator first = begin(); first != last; ++first) {
      if (compare_(value, *first)) return first;
    }
    return last;
  }

  const_iterator upper_bound(const value_type& value) const {
    const_iterator last = end();
    for (const_iterator first = begin(); first != last; ++first) {
      if (compare_(value, *first)) return first;
    }
    return last;
  }

  void swap(RedBlackTree& other) {
    std::swap(this->root_, other.root_);
    std::swap(this->nil_, other.nil_);
    std::swap(this->header_, other.header_);
    std::swap(this->size_, other.size_);
    std::swap(this->node_alloc_, other.node_alloc_);
    std::swap(this->alloc_, other.alloc_);
    std::swap(this->compare_, other.compare_);
  }

  std::pair<iterator, iterator> equal_range(const value_type& value) {
    return std::make_pair(lower_bound(value), upper_bound(value));
  }

  allocator_type get_allocator() const { return alloc_; }
};

// template<class Content, class Compare, class Alloc>
// bool operator<(const RedBlackTree<Content, Compare, Alloc> &lhs, const
// RedBlackTree<Content, Compare, Alloc>& rhs){
//     return(std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
//     rhs.end()));
// };

template <class Content, class Compare, class Alloc>
bool operator>(const RedBlackTree<Content, Compare, Alloc>& lhs,
               const RedBlackTree<Content, Compare, Alloc>& rhs) {
  return (lhs < rhs);
};

template <class Content, class Compare, class Alloc>
bool operator==(const RedBlackTree<Content, Compare, Alloc>& lhs,
                const RedBlackTree<Content, Compare, Alloc>& rhs) {
  return (lhs.size() == rhs.size() &&
          std::equal(lhs.begin(), lhs.end(), rhs.begin()));
};

template <class Content, class Compare, class Alloc>
void swap(const RedBlackTree<Content, Compare, Alloc>& lhs,
          const RedBlackTree<Content, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
};
}  // namespace container

#endif  // RB_TREE_H_