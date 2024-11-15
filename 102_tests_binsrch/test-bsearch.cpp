#include <cmath>
#include <iostream>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n), f(fn), mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};
//sin
class Function1 : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};
//f(x)= -100
class Function2 : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return -100; }
};
//f(x)= 100
class Function3 : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 100; }
};
//f(x)= 3x
class Function4 : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 3 * arg; }
};
//f(x)= -x
class Function5 : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return -arg; };
};
//f(x)= x+2
class Function6 : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 3 * arg + 3; };
};
void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int maxNum;
  if (high > low) {
    maxNum = log(high - low) / log(2) + 1;
  }
  else {
    maxNum = 1;
  }
  CountedIntFn * function = new CountedIntFn(maxNum, f, mesg);
  int result = binarySearchForZero(function, low, high);
  if (result != expected_ans) {
    fprintf(stderr, "ERROR: result of %s is wrong.\n", mesg);
    exit(EXIT_FAILURE);
  }
}
int main() {
  Function1 f1;
  Function2 f2;
  Function3 f3;
  Function4 f4;
  Function5 f5;
  Function6 f6;
  //f(x)=sinx
  check(&f1, 0, 150000, 52359, "Sin");
  //f(x)=-100
  check(&f2, -100, 100, 99, "-100 in range(-100,100)");
  check(&f2, 10, 1000, 999, "-100 in range(10,100)");
  check(&f2, -100, -10, -11, "-100 in range(-100,-10)");
  check(&f2, 0, 0, 0, "-100 in (0,0)");
  //f(x)= 100
  check(&f3, -100, 100, -100, "100 in range(-100,100)");
  check(&f3, 10, 1000, 10, "100 in range(10,100)");
  check(&f3, -100, -10, -100, "100 in range(-100,-10)");
  check(&f3, 0, 0, 0, "100 in (0,0)");
  //f(x)=3x
  check(&f4, -100, 100, 0, "3x in range(-100,100)");
  check(&f4, 10, 1000, 10, "3x in range(10,100)");
  check(&f4, -100, -10, -11, "3x in range(-100,-10)");
  check(&f4, 0, 0, 0, "3x in (0,0)");
  //f(x)=-x
  check(&f5, -100, 100, 0, "-x in range(-100,100)");
  check(&f5, 10, 1000, 999, "-x in range(10,100)");
  check(&f5, -100, -10, -100, "-x in range(-100,-10)");
  check(&f5, 0, 0, 0, "-x in (0,0)");
  //f(x)= 3x+3
  check(&f6, -100, 100, -1, "3x+3 in range(-100,100)");
  check(&f6, 10, 1000, 10, "3x+3 in range(10,1000)");
  check(&f6, -100, -10, -11, "3x+3 in range(-100,-10)");
  check(&f6, 0, 0, 0, "3x+3 in (0,0)");
}
