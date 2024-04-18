#ifndef STACK_H_
#define STACK_H_

#include <iostream>

#include "../Vector/vector.h"

namespace container {
template <class T,
          class Container =
              container::Vector<T> >  // T - тип данных для сохранения в стеке,
                                // Container - тип базового контейнера
                                //используемого для реализации стека, по
                                //умолчанию deque<T>
                                class Stack {
 public:
  typedef T value_type;  //Тип, представляющий тип объекта, который хранится в
                         //виде элемента в stack.
  typedef Container container_type;  // Тип, предоставляющий базовый контейнер
                                     // для принятия stack
  typedef typename Container::size_type
      size_type;  //Целочисленный Typedef без знака, который может представлять
                  //число элементов в stack.
  typedef T &reference;              // Ссылка на T value_type
  typedef const T &const_reference;  // const Ссылка на T value_type
 protected:
  container_type
      _container;  // container на основе которого мы реализуем метод стек, в
                   // следствие чего выполняем все операции с ним.
 public:
  explicit Stack(const container_type &cont = container_type())
      : _container(cont){

        };

  Stack(const Stack &other)
      : _container(other._container){
            // Создание обьекта с помощью другого стека

        };
  
  ~Stack() {}

  Stack &operator=(
      const Stack &other) {  //перегрузка оператора = возвращает текущий обьекта
    _container = other._container;
    return (*this);
  };

  bool empty() const {  //Проверяет, является ли stack пустым.
    return (_container.empty());
  };

  size_type size()
      const {  //Возвращает количество элементов в контейнере stack.
    return (_container.size());
  };

  value_type &top() {  //Возвращает ссылку на элемент в верхней части stack.
    return (_container.back());
  };

  const reference top()
      const {  //Возвращает ссылку на элемент в верхней части stack.
    return (_container.back());
  };

  void push(const value_type &val) {  //Добавляет элемент в верхнюю часть stack.
    _container.push_back(val);
  };

  void swap(Stack &other) { _container.swap(other._container); }

  void pop() {  //Удаляет элемент из верхней части stack.
    _container.pop_back();
  };
  // перегрузка операторов сравнения
  template <class A, class B>
  friend bool operator==(const Stack<A, B> &lhs, const Stack<A, B> &rhs) {
    return (lhs._container == rhs._container);
  }

  template <class A, class B>
  friend bool operator!=(const Stack<A, B> &lhs, const Stack<A, B> &rhs) {
    return (lhs._container != rhs._container);
  }

  template <class A, class B>
  friend bool operator>(const Stack<A, B> &lhs, const Stack<A, B> &rhs) {
    return (lhs._container > rhs._container);
  }

  template <class A, class B>
  friend bool operator<(const Stack<A, B> &lhs, const Stack<A, B> &rhs) {
    return (lhs._container < rhs._container);
  }

  template <class A, class B>
  friend bool operator<=(const Stack<A, B> &lhs, const Stack<A, B> &rhs) {
    return (lhs._container <= rhs._container);
  }

  template <class A, class B>
  friend bool operator>=(const Stack<A, B> &lhs, const Stack<A, B> &rhs) {
    return (lhs._container >= rhs._container);
  }
};
}  // namespace container

#endif  // STACK_H_