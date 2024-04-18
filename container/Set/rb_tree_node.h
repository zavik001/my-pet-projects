#ifndef RB_TREE_NODE_H_
#define RB_TREE_NODE_H_
#include <cstddef>

#include "compare.h"
#include "set.h"
namespace container {
template <class Key, class Compare = container::Compare<Key>>
class TreeNode {
 public:
  using key_type = Key;
  using reference = Key &;
  using const_reference = const Key &;
  using cmp_type = Compare;
  using size_type = size_t;

 public:
  cmp_type comp_;
  TreeNode *parent_;
  TreeNode *left_;
  TreeNode *right_;
  key_type key_;

  TreeNode() : parent_(nullptr), left_(nullptr), right_(nullptr) { ; }

  TreeNode(key_type data, TreeNode *parent = nullptr, TreeNode *left = nullptr,
           TreeNode *right = nullptr)
      : parent_(parent), left_(left), right_(right), key_(data) {
    ;
  }

  TreeNode(const TreeNode &other) = default;

  TreeNode(TreeNode &&other) = default;

  constexpr TreeNode &operator=(const TreeNode &other) = default;

  ~TreeNode() { ; }

 public:
  TreeNode *get_nill() {
    TreeNode *nill = this;
    while (nill->right_ != nullptr) nill = nill->right_;
    return nill;
  }

  TreeNode *get_root() {
    TreeNode *result = this;
    while (result->parent_ != nullptr) {
      result = result->parent_;
    }
    return result;
  }

  TreeNode *get_max() {
    TreeNode *root = get_root();
    while (root->right_ != get_nill()) root = root->right_;
    if (comp_.eq(root->left_->key_, root->key_)) {
      while (root->left_ != get_nill()) root = root->left_;
    }
    return root;
  }

  TreeNode *get_min() {
    TreeNode *root = get_root();
    while (root->left_ != get_nill()) root = root->left_;
    return root;
  }

  TreeNode *get_next_node() {
    TreeNode *node = this;
    //Key temp = node->key_;
    if (node->right_ == get_nill()) {
      node = node->parent_;
    } else {
      node = node->right_;
      while (node->left_ != get_nill()) {
        node = node->left_;
      }
    }
    return node;
  }
};
}  // namespace container
#endif  // RB_TREE_NODE_H_