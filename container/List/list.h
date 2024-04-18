#ifndef LIST_H_
#define LIST_H_
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include "iterator.h"
namespace container {
template <typename T, class Alocator = std::allocator<T>>
class List {
 public:
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using const_reference = const T &;
  using iterator = ListIterator<T>;
  using const_iterator = ListConstIterator<T>;
  using node_pointer = Node<T> *;
  using node_type = Node<T>;
  using alloc_type = Alocator;
  using alloc_rebind = typename alloc_type::template rebind<Node<T>>::other;
  using size_type = std::size_t;

 private:
  node_pointer head_;
  node_pointer tail_;
  node_pointer fake_node_;
  size_type size_;

 protected:
  alloc_rebind alloc_;

 public:
  List()
      : head_(nullptr), tail_(nullptr), fake_node_(new node_type), size_(0) {}

  List(size_type n) : List() {
    for (size_type i = 0; i < n; i++) {
      push_back(value_type());
    }
  }

  List(std::initializer_list<value_type> const &items) : List() {
    for (auto it = items.begin(); it != items.end(); it++) push_back(*it);
  }

  List(const List &other) : List() { copy(other); }

  List(List &&other) : List() { 
    copy(other);
    }

  ~List() {
    this->clear();
    delete fake_node_;
  }

  List &operator=(const List &other) {
    copy(other);
    return *this;
  }

  iterator begin() { return iterator(head_); }

  const_iterator begin() const { return head_; }

  iterator end() { return iterator(fake_node_); }

  const_iterator end() const { return fake_node_; }

  const_reference front() {
    return (head_ != nullptr) ? head_->data_ : fake_node_->data_;
  }

  const_reference back() {
    return (tail_ != nullptr) ? tail_->data_ : fake_node_->data_;
  }

  void copy(const List &other) {
    if (other.size_ != 0) {
      for (auto iter = other.begin(); iter != other.end(); ++iter) {
        push_back(*iter);
      }
    }
  }

  void clear() {
    iterator b = begin();
    while (b != end()) 
    {
      iterator pre = b;
      b++;
      delete pre.node();
      };
  }

  void pop_back() {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    erase(--end());
  }

  void pop_front() {
    if (this->empty()) {
      throw std::out_of_range("List is empty");
    }
    erase(begin());
  }

  void push_back(const_reference value) {
    node_pointer temp = new node_type(value, nullptr, tail_);  // fix
    if (tail_ != nullptr) {
      tail_->next_ = temp;
    }
    if (size_ == 0) {
      head_ = tail_ = temp;
    } 
    else {
      tail_ = temp;
    }
    size_++;
    set_fake();
  }

  void push_front(const_reference value) {
    node_pointer temp = new node_type(value, head_, nullptr);
    if (head_ != nullptr) {
      head_->prev_ = temp;
    }
    if (size_ == 0) {
      head_ = tail_ = temp;
    } else {
      head_ = temp;
    }
    size_++;
    set_fake();
  }

  bool empty() { return (head_ == nullptr); }

  size_type size() { return size_; }

  size_type max_size() { return alloc_.max_size(); }

  iterator insert(iterator pos, const_reference value) {
    iterator res;
    if (pos == begin()) {
      push_front(value);
      res = begin();
    } else if (pos == end()) {
      push_back(value);
      res = tail_->prev_;
    } else {
      node_pointer node_for_pos = new node_type{value, head_, nullptr};
      if (pos.node()->prev_) {
        pos.node()->prev_->next_ = node_for_pos;
        node_for_pos->prev_ = pos.node()->prev_;
      }
      pos.node()->prev_ = node_for_pos;
      node_for_pos->next_ = pos.node();
      size_++;
      res = iterator(node_for_pos);
    }
    return res;
  }

    void erase(iterator pos) {
    if (size_ == 0) {
      throw std::out_of_range("Iterator is out of range");
    }
    node_pointer node = get_node_by_pos(pos.node());
    node_pointer node_next = node->next_;
    node_pointer node_back = node->prev_;
    if (size_ > 1 && node_next != fake_node_) {
      node_next->prev_ = node_back;
    }
    if (size_ > 1 && node_back != fake_node_) {
      node_back->next_ = node_next;
    }
    if (pos == begin()) {
      tail_ = node_next;
    }
    if (pos == --end()) {
      tail_ = node_back;
    }
    size_--;
    set_fake();
    delete pos.node();
  }

  void swap(List &other) {
    List new_list(other);
    other = *this;
    *this = new_list;
  }

  void merge(List &other) {
    iterator it_this = begin();
    value_type prev_value = other.front();
    for (auto it_other = other.begin(); it_other != other.end();
         prev_value = *it_other) {
      if (*it_other < prev_value || *it_this > *it_other) {
        insert(it_this, *it_other);
        ++it_other;
      } else {
        ++it_this;
      }
    }
 }

  void splice(iterator pos, List &other) {
    iterator this_it = begin();
    for (; this_it != pos; ++this_it) {
      ;
    }
    for (auto it = other.begin(); it != other.end(); ++it) insert(this_it, *it);
  }

  void reverse() {
    List reverse_list;
    for (auto it = begin(); it != end(); ++it) reverse_list.push_front(*it);
    *this = reverse_list;
  }

  void unique() {
    iterator it_next = ++(begin());
    for (auto it = begin(); it_next != end(); ++it_next) {
      if (*it == *it_next) {
        erase(it_next);
      } else {
        ++it;
      }
    }
  }

  void sort() {
    if (!empty()) {
      head_ = sorting(head_);
    }
  }

  node_pointer merge(node_pointer a, node_pointer b) {
    if (a == tail_) return b;
    if (b == tail_) return a;
    node_pointer c;
    if ((a->data_) < (b->data_)) {
      c = a;
      c->next_ = merge(a->next_, b);
    } else {
      c = b;
      c->next_ = merge(a, b->next_);
    }
    return c;
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&...args) {
    iterator it(const_cast<node_pointer>(pos.base()));
    return insert(it, value_type(std::forward<Args>(args)...));
  }

  template <class... Args>
  void emplace_back(Args &&...args) {
    push_back(value_type(std::forward<Args>(args)...));
  }

  template <class... Args>
  void emplace_front(Args &&...args) {
    push_front(value_type(std::forward<Args>(args)...));
  }

 private:
  node_pointer sorting(node_pointer head) {
    if (head == tail_ || head->next_ == tail_) {
      return head;
    }
    node_pointer mid = split_list(head);
    node_pointer a = head;
    node_pointer b = mid->next_;
    mid->next_ = tail_;

    a = sorting(a);
    b = sorting(b);
    return merge(a, b);
  }
  node_pointer get_node_by_pos(const_iterator pos) {
    node_pointer result = head_;
    for (auto it = begin(); it != pos.node(); ++it) result = result->next_;
    return result;
  }
  node_pointer split_list(node_pointer head) {
    node_pointer fast = head->next_;
    node_pointer slow = head;

    while (fast != tail_ && fast->next_ != tail_) {
      fast = fast->next_->next_;
      slow = slow->next_;
    }
    return slow;
  }
  void set_fake() {
    if (head_ != nullptr && tail_ != nullptr) {
      head_->prev_ = fake_node_;
      tail_->next_ = fake_node_;
    }
    fake_node_->prev_ = tail_;
    fake_node_->next_ = head_;
  }
};
}  
#endif  // LIST_H_