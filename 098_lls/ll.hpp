#ifndef __LL_HPP__
#define __LL_HPP__

#include <assert.h>

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>

class OOB : public std::exception {
 public:
  const char * what() const throw() { return "Out of boundary"; }
};

template<typename T>
class LinkedList {
 private:
  class Node {
   public:
    T data;
    Node * next;
    Node * prev;

    Node(const T & d, Node * n = NULL, Node * p = NULL) : data(d), next(n), prev(p) {}
  };

  Node * head;
  Node * tail;
  int size;

  void clear() {
    while (head) {
      Node * temp = head;
      head = head->next;
      delete temp;
    }
    tail = NULL;
    size = 0;
  }

  void copyFrom(const LinkedList & rhs) {
    for (Node * curr = rhs.head; curr != NULL; curr = curr->next) {
      addBack(curr->data);
    }
  }

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}

  LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(0) { copyFrom(rhs); }

  LinkedList & operator=(const LinkedList & rhs) {
    if (this != &rhs) {
      clear();
      copyFrom(rhs);
    }
    return *this;
  }

  ~LinkedList() { clear(); }

  void addFront(const T & item) {
    Node * n = new Node(item, head);
    if (head) {
      head->prev = n;
    }
    else {
      tail = n;
    }
    head = n;
    size++;
  }

  void addBack(const T & item) {
    Node * n = new Node(item, NULL, tail);
    if (tail) {
      tail->next = n;
    }
    else {
      head = n;
    }
    tail = n;
    size++;
  }

  bool remove(const T & item) {
    for (Node * curr = head; curr != NULL; curr = curr->next) {
      if (curr->data == item) {
        if (curr->prev) {
          curr->prev->next = curr->next;
        }
        else {
          head = curr->next;
        }

        if (curr->next) {
          curr->next->prev = curr->prev;
        }
        else {
          tail = curr->prev;
        }

        delete curr;
        size--;
        return true;
      }
    }
    return false;
  }

  T & operator[](int index) {
    if (index < 0 || index >= size) {
      throw OOB();
    }
    Node * curr = head;
    for (int i = 0; i < index; ++i) {
      curr = curr->next;
    }
    return curr->data;
  }

  const T & operator[](int index) const {
    if (index < 0 || index >= size) {
      throw OOB();
    }
    Node * curr = head;
    for (int i = 0; i < index; ++i) {
      curr = curr->next;
    }
    return curr->data;
  }

  int find(const T & item) const {
    int index = 0;
    for (Node * curr = head; curr != NULL; curr = curr->next, ++index) {
      if (curr->data == item) {
        return index;
      }
    }
    return -1;
  }

  int getSize() const { return size; }

  friend void testList(void);
};

#endif
