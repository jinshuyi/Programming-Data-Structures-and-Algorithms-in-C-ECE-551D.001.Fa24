#include <assert.h>

#include <cstdlib>
#include <iostream>

#include "il.hpp"

class Tester {
 public:
  void testDefCtor() {
    IntList il;
    assert(il.head == NULL && il.tail == NULL && il.getSize() == 0);
  }

  void testAddFront() {
    IntList il;
    il.addFront(0);
    assert(il[0] == 0 && il.head->data == 0 && il.tail->data == 0 &&
           il.head->prev == NULL && il.head->next == NULL && il.tail->prev == NULL &&
           il.tail->next == NULL && il.getSize() == 1);

    il.addFront(1);
    assert(il[0] == 1 && il.head->data == 1 && il.head->next != NULL &&
           il.head->next->data == 0 && il.head->prev == NULL && il.tail->data == 0 &&
           il.tail->next == NULL && il.tail->prev != NULL && il.tail->prev->data == 1 &&
           il.getSize() == 2);

    for (int i = 2; i < 100; i++) {
      il.addFront(i);
      assert(il[0] == i && il.head->data == i && il.tail->data == 0 &&
             il.getSize() == i + 1);
    }

    IntList nil1(il);
    nil1.addFront(1);
    nil1.tail->data = 1;
    assert(nil1[0] == 1 && nil1[100] == 1 && nil1.head->data == 1 &&
           nil1.tail->data == 1 && nil1.getSize() == 101 && il[0] == 99 && il[99] == 0 &&
           il.getSize() == 100 && il.head->data == 99 && il.tail->data == 0);

    IntList nil2 = il;
    nil2.addFront(1);
    nil2.tail->data = 1;
    assert(nil2[0] == 1 && nil2[100] == 1 && nil2.head->data == 1 &&
           nil2.tail->data == 1 && nil2.getSize() == 101 && il[0] == 99 && il[99] == 0 &&
           il.getSize() == 100 && il.head->data == 99 && il.tail->data == 0);
  }

  void testAddBack() {
    IntList il;
    il.addBack(0);
    assert(il.head != NULL && il[0] == 0 && il.head->data == 0 && il.tail->data == 0 &&
           il.head->prev == NULL && il.head->next == NULL && il.tail->prev == NULL &&
           il.tail->next == NULL && il.getSize() == 1);

    il.addBack(1);
    assert(il[0] == 0 && il[1] == 1 && il.head->data == 0 && il.head->next != NULL &&
           il.head->next->data == 1 && il.head->prev == NULL && il.tail->data == 1 &&
           il.tail->next == NULL && il.tail->prev != NULL && il.tail->prev->data == 0 &&
           il.getSize() == 2);

    for (int i = 2; i < 100; i++) {
      il.addBack(i);
      assert(il[i] == i && il.head->data == 0 && il.tail->data == i &&
             il.getSize() == i + 1);
    }

    IntList nil1(il);
    nil1.addBack(1);
    nil1.head->data = 1;
    assert(nil1[0] == 1 && nil1[100] == 1 && nil1.head->data == 1 &&
           nil1.tail->data == 1 && nil1.getSize() == 101 && il[0] == 0 && il[99] == 99 &&
           il.head->data == 0 && il.tail->data == 99 && il.getSize() == 100);

    IntList nil2 = il;
    nil2.addBack(1);
    nil2.head->data = 1;
    assert(nil2[0] == 1 && nil2[100] == 1 && nil2.head->data == 1 &&
           nil2.tail->data == 1 && nil2.getSize() == 101 && il[0] == 0 && il[99] == 99 &&
           il.head->data == 0 && il.tail->data == 99 && il.getSize() == 100);

    nil1 = il;
    assert(nil1[0] == 0 && nil1[99] == 99 && nil1.head->data == 0 &&
           nil1.tail->data == 99 && nil1.getSize() == 100);
  }

  void testRemove() {
    IntList il;
    il.addBack(0);
    assert(il.head != NULL && il[0] == 0 && il.head->data == 0 && il.tail->data == 0 &&
           il.head->prev == NULL && il.head->next == NULL && il.tail->prev == NULL &&
           il.tail->next == NULL && il.getSize() == 1);

    assert(il.remove(0) == true && il.head == NULL && il.tail == NULL &&
           il.getSize() == 0);

    il.addFront(0);
    il.addBack(1);
    assert(il[0] == 0 && il[1] == 1 && il.head->data == 0 && il.head->next != NULL &&
           il.head->next->data == 1 && il.head->prev == NULL && il.tail->data == 1 &&
           il.tail->next == NULL && il.tail->prev != NULL && il.tail->prev->data == 0 &&
           il.getSize() == 2);

    assert(il.remove(2) == false && il[0] == 0 && il[1] == 1 && il.head->data == 0 &&
           il.head->next != NULL && il.head->next->data == 1 && il.head->prev == NULL &&
           il.tail->data == 1 && il.tail->next == NULL && il.tail->prev != NULL &&
           il.tail->prev->data == 0 && il.getSize() == 2);

    il.remove(1);
    assert(il.head != NULL && il[0] == 0 && il.head->data == 0 && il.tail->data == 0 &&
           il.head->prev == NULL && il.head->next == NULL && il.tail->prev == NULL &&
           il.tail->next == NULL && il.getSize() == 1);

    il.addFront(1);
    il.remove(1);
    assert(il.head != NULL && il[0] == 0 && il.head->data == 0 && il.tail->data == 0 &&
           il.head->prev == NULL && il.head->next == NULL && il.tail->prev == NULL &&
           il.tail->next == NULL && il.getSize() == 1);

    il.addBack(1);
    il.addBack(2);
    il.remove(1);
    assert(il[0] == 0 && il[1] == 2 && il.head->data == 0 && il.head->next != NULL &&
           il.head->next->data == 2 && il.head->prev == NULL && il.tail->data == 2 &&
           il.tail->next == NULL && il.tail->prev != NULL && il.tail->prev->data == 0 &&
           il.getSize() == 2);
  }
};

int main(void) {
  Tester t;
  t.testDefCtor();
  t.testAddFront();
  t.testAddBack();
  t.testRemove();
  return EXIT_SUCCESS;
}
