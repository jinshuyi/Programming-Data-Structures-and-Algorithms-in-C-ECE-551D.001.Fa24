#include <iostream>
#include <stdexcept>
#include <utility>

#include "set.h"

using namespace std;

template<typename T>
class BstSet : public Set<T> {
  class Node {
   public:
    T data;
    Node * left;
    Node * right;
    Node(T t) : data(t), left(NULL), right(NULL) {}
  };

  Node * root;

  void freeBst(Node * node) {
    if (node) {
      freeBst(node->left);
      freeBst(node->right);
      delete node;
    }
  }

  Node * copyTree(const Node * source) {
    if (!source)
      return NULL;
    Node * newNode = new Node(source->data);
    newNode->left = copyTree(source->left);
    newNode->right = copyTree(source->right);
    return newNode;
  }

  Node * find(const T & key) const {
    Node * current = root;
    while (current) {
      if (current->data == key)
        return current;
      current = (current->data > key) ? current->left : current->right;
    }
    return NULL;
  }

 public:
  BstSet() : root(NULL) {}

  BstSet(const BstSet & rhs) : root(copyTree(rhs.root)) {}

  BstSet & operator=(BstSet rhs) {
    swap(root, rhs.root);
    return *this;
  }

  virtual void add(const T & key) {
    Node ** current = &root;
    while (*current) {
      if ((*current)->data == key)
        return;
      current = ((*current)->data > key) ? &(*current)->left : &(*current)->right;
    }
    *current = new Node(key);
  }

  virtual bool contains(const T & key) const { return find(key) != NULL; }

  virtual void remove(const T & key) {
    Node ** target = &root;
    while (*target) {
      if ((*target)->data == key)
        break;
      target = ((*target)->data > key) ? &(*target)->left : &(*target)->right;
    }

    if (!*target)
      return;

    Node * toDelete = *target;
    if (!toDelete->left) {
      *target = toDelete->right;
    }
    else if (!toDelete->right) {
      *target = toDelete->left;
    }
    else {
      Node ** pred = &toDelete->left;
      while ((*pred)->right)
        pred = &(*pred)->right;
      (*target)->data = (*pred)->data;
      toDelete = *pred;
      *pred = (*pred)->left;
    }
    delete toDelete;
  }

  virtual ~BstSet() { freeBst(root); }
};
