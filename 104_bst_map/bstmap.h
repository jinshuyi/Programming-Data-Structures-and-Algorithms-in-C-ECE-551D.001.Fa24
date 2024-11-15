#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <utility>

#include "map.h"
using namespace std;
template<typename K, typename V>
class BstMap : public Map<K, V> {
  class Node {
   public:
    pair<K, V> data;
    Node * left;
    Node * right;
    Node(K k, V v) : data(k, v), left(NULL), right(NULL) {}
  };

  Node * root;

  void copyTree(Node ** dest, const Node * source) {
    if (source) {
      *dest = new Node(source->data.first, source->data.second);
      copyTree(&(*dest)->left, source->left);
      copyTree(&(*dest)->right, source->right);
    }
  }

  Node ** find(const K & key) {
    Node ** current = &root;
    while (*current) {
      if ((*current)->data.first == key)
        return current;
      current = ((*current)->data.first > key) ? &(*current)->left : &(*current)->right;
    }
    return NULL;
  }

  void freeBst(Node * node) {
    if (node) {
      freeBst(node->left);
      freeBst(node->right);
      delete node;
    }
  }

 public:
  BstMap() : root(NULL) {}

  BstMap(const BstMap & other) : root(NULL) { copyTree(&root, other.root); }

  BstMap & operator=(BstMap other) {
    swap(root, other.root);
    return *this;
  }

  virtual void add(const K & key, const V & value) {
    Node ** current = &root;
    while (*current) {
      if ((*current)->data.first == key) {
        (*current)->data.second = value;
        return;
      }
      current = ((*current)->data.first > key) ? &(*current)->left : &(*current)->right;
    }
    *current = new Node(key, value);
  }

  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    const Node * current = root;
    while (current) {
      if (current->data.first == key)
        return current->data.second;
      current = (current->data.first > key) ? current->left : current->right;
    }
    throw invalid_argument("Key does not exist");
  }

  virtual void remove(const K & key) {
    Node ** target = find(key);
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
      swap((*pred)->data, toDelete->data);
      toDelete = *pred;
      *pred = (*pred)->left;
    }
    delete toDelete;
  }

  virtual ~BstMap() { freeBst(root); }
};
