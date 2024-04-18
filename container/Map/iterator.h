#ifndef ITERATOR_H_
#define ITERATOR_H_
#include <iostream>
namespace container {
template <class Value>
struct Node {
 public:
  explicit Node(Value *scrval = 0)
      : value_(scrval),
        parent_(0),
        left_(0),
        right_(0),
        is_black(false),
        is_nil(false) {}

  Value *value_;
  Node *parent_;
  Node *left_;
  Node *right_;
  bool is_black;
  bool is_nil;

  Node(const Node &other) {
    this->value_ = other.value_;
    this->parent_ = other.parent_;
    this->left_ = other.left_;
    this->right_ = other.right_;
    this->is_black = other.is_black;
    this->is_nil = other.is_nil;
  };

  Node &operator=(const Node &other) {
    this->value_ = other.value_;
    this->parent_ = other.parent_;
    this->left_ = other.left_;
    this->right_ = other.right_;
    this->is_black = other.is_black;
    this->is_nil = other.is_nil;
    return *this;
  };

  virtual ~Node() {}
};

template <typename T>
class TreeIterator {
 public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef std::ptrdiff_t difference_type;
  typedef Node<T> *node_pointer;

 private:
  node_pointer _node;

  node_pointer tree_min(node_pointer n) const {
    while (n->left_ != NULL && !n->left_->is_nil) {
      n = n->left_;
    }
    return n;
  }

  node_pointer tree_max(node_pointer n) const {
    while (!n->right_->is_nil) {
      n = n->right_;
    }
    return n;
  }

 public:
  TreeIterator(){};

  TreeIterator(void *node) : _node(static_cast<node_pointer>(node)) {}

  TreeIterator(const TreeIterator<T> &other) { *this = other; }

  TreeIterator &operator=(const TreeIterator<T> &other) {
    this->_node = other.node();
    return *this;
  }

  reference operator*() const { return *(_node->value_); }

  pointer operator->() const { return _node->value_; }

  TreeIterator &operator++() {
    if (_node->right_ && !_node->right_->is_nil)
      _node = tree_min(_node->right_);
    else {
      node_pointer n = _node->parent_;
      while (n != NULL && _node == n->right_) {
        _node = n;
        n = n->parent_;
      }
      _node = n;
    }
    return *this;
  }

  TreeIterator operator++(int) {
    TreeIterator<value_type> temp = *this;
    if (!_node->right_->is_nil) {
      _node = tree_min(_node->right_);
    } else {
      node_pointer y = _node->parent_;
      while (y != NULL && _node == y->right_) {
        _node = y;
        y = y->parent_;
      }
      _node = y;
    }
    return temp;
  }

  TreeIterator &operator--() {
    if (_node->left_ && !_node->left_->is_nil)
      _node = tree_max(_node->left_);
    else {
      node_pointer n = _node->parent_;
      while (n != NULL && _node == n->left_) {
        _node = n;
        n = n->parent_;
      }
      _node = n;
    }
    return *this;
  }

  TreeIterator operator--(int) {
    TreeIterator<value_type> temp = *this;
    if (_node->left_ && !_node->left_->is_nil)
      _node = tree_max(_node->left_);
    else {
      node_pointer n = _node->parent_;
      while (n != NULL && _node == n->left_) {
        _node = n;
        n = n->parent_;
      }
      _node = n;
    }
    return temp;
  }

  node_pointer node() const { return _node; }
};

template <typename A, typename B>
bool operator==(const TreeIterator<A> &lhs, const TreeIterator<B> &rhs) {
  return lhs.node() == rhs.node();
}

template <typename A, typename B>
bool operator!=(const TreeIterator<A> &lhs, const TreeIterator<B> &rhs) {
  return lhs.node() != rhs.node();
}
}  // namespace container
#endif  // ITERATOR_H_