#ifndef QUEUE_H_
#define QUEUE_H_

#include <initializer_list>
#include <stdexcept>

namespace container {
template <class T>
class Queue {
 private:
  class Node {
   public:
    Node *next;
    T data;

    Node(T data = T(), Node *next = nullptr) {
      this->data = data;
      this->next = next;
    }
  };

  Node *first_, *last_;
  size_t size_;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

 public:
  Queue() {
    this->size_ = 0;
    this->first_ = nullptr;
    this->last_ = nullptr;
  }

  Queue(std::initializer_list<value_type> const &items) {
    for (auto it = items.begin(); it != items.end(); it++) this->push(*it);
  }

  Queue(const Queue &q) : Queue() { this->copy(q); }

  Queue(Queue &&q) : Queue() { q.clear(); }

  ~Queue() { this->clear(); }

  Queue<value_type> &operator=(Queue &q) {
    this->copy(q);
    return *this;
  }

  const_reference front() {
    if (this->first_ == nullptr)
      throw std::out_of_range("Error: Queue is empty");
    return this->first_->data;
  }

  const_reference back() {
    if (this->last_ == nullptr)
      throw std::out_of_range("Error: Queue is empty");
    return this->last_->data;
  }

  bool empty() { return (this->size_ == 0); }

  size_type size() { return this->size_; }

  void push(const_reference value) {
    Node *node = new Node(value, nullptr);
    if (this->first_ == nullptr) this->first_ = node;
    if (this->last_ != nullptr) this->last_->next = node;
    this->last_ = node;
    this->size_++;
  }

  void pop() {
    if (this->first_ != nullptr) {
      Node *node = this->first_->next;
      delete this->first_;
      this->first_ = node;
      this->size_--;
      if (size_ < 2) this->last_ = node;
    }
  }

  void swap(Queue &other) {
    Queue<value_type> new_queue(other);
    other = *this;
    *this = new_queue;
  }

  template <class... Args>
  void emplace_back(Args &&...args) {
    return this->push(value_type(std::forward<Args>(args)...));
  }

  void clear() {
    size_type const_size = this->size_;
    for (size_type i = 0; i < const_size; i++) this->pop();
  }

  void copy(const Queue &other) {
    this->clear();
    Node *node = other.first_;
    for (size_type i = 0; i < other.size_; i++) {
      this->push(node->data);
      node = node->next;
    }
  }
};
}  // namespace container

#endif  //  QUEUE_H_