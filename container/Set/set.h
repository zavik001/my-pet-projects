#ifndef SET_H_
#define SET_H_
#include <initializer_list>
#include <iostream>
#include <utility>

#include "../Vector/vector.h"
#include "compare.h"
#include "iterator.h"
#include "rb_tree_node.h"

namespace container {
template <typename Key, class Compare = container::Compare<Key>>
class Set {
 public:
  using key_type = Key;
  using reference = Key &;
  using const_reference = const Key &;
  using cmp_type = Compare;
  using iterator = TreeIterator<key_type, cmp_type>;
  using const_iterator = TreeConstIterator<key_type, cmp_type>;
  using size_type = size_t;
  using node_type = TreeNode<Key, cmp_type>;
  using node_pointer = TreeNode<Key, cmp_type> *;

 protected:
  cmp_type comp_;
  node_pointer root_;
  node_pointer nill_ = new node_type();
  size_type size_ = 0;
  std::allocator<key_type> alloc_;

 public:
  Set() : root_(nullptr) { ; }

  Set(std::initializer_list<key_type> const &items) : Set() {
    for (auto it = items.begin(); it != items.end(); it++) insert(*it);
  }

  Set(const Set &other) : Set() { copy_(other.root_, other.nill_); }

  Set(Set &&other) : Set() {
    delete nill_;
    root_ = other.root_;
    size_ = other.size_;
    nill_ = other.nill_;
    other.root_ = nullptr;
    other.nill_ = nullptr;
    other.size_ = 0;
  }

  ~Set() {
    if (size_ != 0) clear();
    if (nill_) delete nill_;
  }

  bool empty() const { return size_ == 0; }
  size_type size() const noexcept { return size_; }
  size_type max_size() const noexcept { return alloc_.max_size(); }
  std::pair<iterator, bool> insert(const key_type &value) {
    // если root пустой , то вставляем на его место
    // если не пустой, тогда 1: если элемент существует , то его не добавляем ,
    // возвращаем false через пару 2: если такого элемента нет, то его
    // закидываем по правилам
    return Insert_new_element(value);
  }

  bool contains(const key_type &value) const {
    auto result = search_key(value);
    return result.second;
  }
  iterator begin() const {
    return iterator(size_ == 0 ? root_ : iterator(tree_min(root_)));
  }
  const_iterator cbegin() const {
    return const_iterator(size_ == 0 ? root_ : const_iterator(tree_min(root_)));
  }
  iterator end() const { return iterator(nill_); }
  const_iterator cend() const { return const_iterator(nill_); }
  void clear() {
    Clear_tree(this->root_);
    root_ = nullptr;
    size_ = 0;
  }

  void swap(Set &other) {
    node_pointer tmp_root = other.root_;
    node_pointer tmp_nill = other.nill_;
    size_type tmp_size = other.size_;
    other.root_ = this->root_;
    other.nill_ = this->nill_;
    other.size_ = this->size_;
    this->root_ = tmp_root;
    this->nill_ = tmp_nill;
    this->size_ = tmp_size;
  }
  void merge(Set &other) {
    //ищем уникальные элементы из второго дерева
    //если есть , то отправляем в 1 дерево
    if (!other.empty()) {
      //if (other.root_ != nullptr) {
        for (auto it_1 = other.begin(); it_1 != other.end(); it_1++) {
          if (!this->contains(*it_1)) {
            this->insert(*it_1);
            other.erase(it_1);
            
            other.size_--;
          } else {
#if defined container_MULTISET_H_
            this->insert(*it_1);
            other.size_--;
#endif
          }
        } 
      //}
    }
  }

  iterator find(const Key &value) {
    return search_key(value).second == true ? search_key(value).first : end();
  }

  void erase(iterator pos) {
    node_pointer y = pos.base();
    if (is_nill(y->left_)) {
      transplant(y, y->right_);
    } else if (is_nill(y->right_)) {
      transplant(y, y->left_);
    } else {
      node_pointer z = y;
      y = tree_min(z->right_);
      if (y->parent_ != z) {
        transplant(y, y->right_);
        y->right_ = z->right_;
        z->right_->parent_ = y;
      }
      transplant(z, y);
      y->left_ = z->left_;
      y->left_->parent_ = y;
    }
    delete y;
  }

  void operator=(Set &&other) {
    Clear_tree(root_);
    delete nill_;
    root_ = other.root_;
    size_ = other.size_;
    nill_ = other.nill_;
    other.root_ = nullptr;
    other.nill_ = nullptr;
    other.size_ = 0;
  }

  template <class... Args>
  Vector<std::pair<iterator, bool>> emplace(Args &&...args) {
    Vector<std::pair<iterator, bool>> result;
    result.push_back(insert(key_type(std::forward<Args>(args)...)));
    return result;
  }
  // helpers
 public:
  void transplant(node_pointer position, node_pointer node) {
    if (position == root_) {
      root_ = node;
    } else if (position == position->parent_->left_) {
      position->parent_->left_ = node;
    } else
      position->parent_->right_ = node;
    node->parent_ = position->parent_;
    size_--;
  }

  node_pointer tree_min(node_pointer n) const {
    while (n != NULL && !is_nill(n->left_)) n = n->left_;
    return n;
  }

  bool is_nill(node_pointer node) const { return node == nill_; }
  std::pair<iterator, bool> search_key(const key_type &value) const {
    bool result = false;
    node_pointer node = root_;
    while (node && node != nill_) {
      if (comp_.lt(value, node->key_)) {
        node = node->left_;
      } else if (comp_.gt(value, node->key_)) {
        node = node->right_;
      } else if (comp_.eq(value, node->key_)) {
        result = true;
        break;
      }
    }
    auto res_iter(node);
    std::pair<iterator, bool> res_search = {res_iter, result};
    return res_search;
  }

  std::pair<iterator, bool> Insert_new_element(const key_type &value) {
    std::pair<iterator, bool> result = {root_, false};
    node_pointer new_element = new node_type(value, nullptr, nill_, nill_);
    if (empty()) {
      result = insert_to_null_tree(new_element);
    } else {
      result = Insert_new_element_to_nonul_tree(root_, new_element);
    }
    return result;
  }

  std::pair<iterator, bool> insert_to_null_tree(node_pointer new_element) {
    std::pair<iterator, bool> result = {root_, false};
    root_ = new_element;
    size_++;
    nill_->parent_ = root_;
    iterator new_insert_iterator(new_element);
    result = {new_insert_iterator, true};
    return result;
  }

  std::pair<iterator, bool> Insert_new_element_to_nonul_tree(
      node_pointer root, node_pointer new_element) {
    std::pair<iterator, bool> result = {root, false};
    if (!contains(new_element->key_)) {
      if (comp_.gt(new_element->key_, root->key_)) {
        result = insert_to_left(root, new_element);
      } else {
        result = insert_to_right(root, new_element);
      }
    } else {
      if (comp_.eq(new_element->key_, root->key_)) {
        result = insert_to_right(root, new_element);
#if defined container_MULTISET_H_
      } else {
        if (comp_.gt(new_element->key_, root->key_)) {
          root = root->right_;
          result = Insert_new_element_to_nonul_tree(root, new_element);
        } else {
          root = root->left_;
          result = Insert_new_element_to_nonul_tree(root, new_element);
        }
#endif
      }
    }
    return result;
  }

  std::pair<iterator, bool> insert_to_left(node_pointer root,
                                           node_pointer new_element) {
    std::pair<iterator, bool> result = {root, false};
    if (root->right_ == nill_) {
      root->right_ = new_element;
      new_element->parent_ = root;
      size_++;
      iterator new_1(root->right_);
      result = {new_1, true};
    } else {
      node_pointer next_ = root->right_;
      Insert_new_element_to_nonul_tree(next_, new_element);
    }
    return result;
  }
  std::pair<iterator, bool> insert_to_right(node_pointer root,
                                            node_pointer new_element) {
    std::pair<iterator, bool> result = {root, false};
    if (root->left_ == nill_) {
      root->left_ = new_element;
      new_element->parent_ = root;
      size_++;
      iterator new_2(root->left_);
      result = {new_2, true};
    } else {
      node_pointer next_1 = root->left_;
      Insert_new_element_to_nonul_tree(next_1, new_element);
    }
    return result;
  }

  void Clear_tree(node_pointer node) {
    if (node != nill_ && node != nullptr) {
      if (node->left_ != nill_) Clear_tree(node->left_);
      if (node->right_ != nill_) Clear_tree(node->right_);
      delete node;
    }
  }

  void copy_(node_pointer node, node_pointer nill) {
    if (node != nullptr && node != nill) {
      if (node->left_ != nill) copy_(node->left_, nill);
      if (node->right_ != nill) copy_(node->right_, nill);
      insert(node->key_);
    }
  }
};
}  // namespace container
#endif  // SET_H_