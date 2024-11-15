#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <cstdlib>
#include <iostream>

template<typename R, typename A>
class Function {
 public:
  virtual R invoke(A arg) = 0;
  virtual ~Function() {}
};

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  while (low < high - 1) {
    int mid = low + (high - low) / 2;
    int res = f->invoke(mid);

    if (res == 0) {
      return mid;
    }
    else if (res > 0) {
      high = mid;
    }
    else {
      low = mid;
    }
  }
  return low;
}
#endif
