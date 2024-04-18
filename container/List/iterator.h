#ifndef ITERATOR_H_
#define ITERATOR_H_
namespace container {
template <typename T>
struct Node {
  Node<T> *prev_;
  Node<T> *next_;
  T data_;
  Node(T data = T(), Node *next = nullptr, Node *prev = nullptr) {
    this->data_ = data;
    this->next_ = next;
    this->prev_ = prev;
  }
};
template <class T>
class ListConstIterator {
 public:
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using node_pointer = Node<value_type> *;

 private:
  node_pointer iterator_;

 public:
  ListConstIterator() : iterator_(nullptr) {}
  ListConstIterator(node_pointer other_iterator) : iterator_(other_iterator) {}
  ListConstIterator(const ListConstIterator &other_iterator)
      : iterator_(other_iterator.iterator_) {}
  ListConstIterator(ListConstIterator &&other_iterator)
      : iterator_(other_iterator.iterator_) {}
  ~ListConstIterator() {}

  ListConstIterator &operator++() {
    this->iterator_ = this->iterator_->next_;
    return *this;
  }
  ListConstIterator &operator++(int) {
    this->iterator_ = this->iterator_->next_;
    return *this;
  }
  ListConstIterator &operator--() {
    this->iterator_ = this->iterator_->prev_;
    return *this;
  }
  bool operator==(const ListConstIterator &iter) {
    return iterator_->data_ == iter.iterator_->data_;
  }
  bool operator!=(const ListConstIterator &iter) {
    return iterator_ != iter.iterator_;
  }
  void operator=(const ListConstIterator &it) { iterator_ = it.iterator_; }
  void operator=(ListConstIterator &&it) { iterator_ = it.iterator_; }
  reference operator*() { return iterator_->data_; }

  ListConstIterator operator+(int value) {
    ListConstIterator tmp(*this);
    return (tmp += value);
  }

  ListConstIterator operator-(int value) {
    ListConstIterator tmp(*this);
    return (tmp -= value);
  }

  ListConstIterator &operator+=(int value) {
    for (int i = 0; i < value; i++) {
      iterator_ = iterator_->next_;
    }
    return *this;
  }

  ListConstIterator &operator-=(int value) {
    for (int i = 0; i < value; i++) {
      iterator_ = iterator_->prev_;
    }
    return *this;
  }

  pointer operator->() { return iterator_->data_; }

  node_pointer node() {
    node_pointer pointer = iterator_;
    return pointer;
  }
};
template <class T>
class ListIterator {
 public:
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using node_pointer = Node<value_type> *;
  using const_iterator = ListConstIterator<T>;

 private:
  node_pointer iterator_;

 public:
  ListIterator() : iterator_(nullptr) {}
  ListIterator(node_pointer other_iterator) : iterator_(other_iterator) {}
  ListIterator(const ListIterator &other_iterator)
      : iterator_(other_iterator.iterator_) {}
  ListIterator(ListIterator &&other_iterator)
      : iterator_(other_iterator.iterator_) {}
  ~ListIterator() {}

  ListIterator &operator++() {
    this->iterator_ = this->iterator_->next_;
    return *this;
  }
  ListIterator &operator++(int) {
    this->iterator_ = this->iterator_->next_;
    return *this;
  }
  ListIterator &operator--() {
    this->iterator_ = this->iterator_->prev_;
    return *this;
  }
  bool operator==(const ListIterator &iter) {
    return iterator_->data_ == iter.iterator_->data_;
  }
  bool operator!=(const ListIterator &iter) {
    return iterator_ != iter.iterator_;
  }
  void operator=(const ListIterator &it) { iterator_ = it.iterator_; }
  void operator=(ListIterator &&it) { iterator_ = it.iterator_; }
  reference operator*() { return iterator_->data_; }

  ListIterator operator+(int value) {
    ListIterator tmp(*this);
    return (tmp += value);
  }

  ListIterator operator-(int value) {
    ListIterator tmp(*this);
    return (tmp -= value);
  }

  ListIterator &operator+=(int value) {
    for (int i = 0; i < value; i++) {
      iterator_ = iterator_->next_;
    }
    return *this;
  }

  ListIterator &operator-=(int value) {
    for (int i = 0; i < value; i++) {
      iterator_ = iterator_->prev_;
    }
    return *this;
  }

  pointer operator->() { return iterator_->data_; }

  node_pointer node() {
    node_pointer pointer = iterator_;
    return pointer;
  }
};
}  // namespace container
#endif  // ITERATOR_H_