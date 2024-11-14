#ifndef __POLY_H__
#define __POLY_H__
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
template<typename NumT>
class Polynomial {
 private:
  vector<NumT> coefficient;

 public:
  Polynomial() : coefficient(1, NumT()) {}
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result;

    size_t maxSize = std::max(coefficient.size(), rhs.coefficient.size());
    result.coefficient.resize(maxSize, NumT());

    for (size_t i = 0; i < maxSize; ++i) {
      if (i < coefficient.size())
        result.coefficient[i] += coefficient[i];
      if (i < rhs.coefficient.size())
        result.coefficient[i] += rhs.coefficient[i];
    }
    return result;
  }

  Polynomial operator-() const {
    Polynomial result = *this;
    for (size_t i = 0; i < result.coefficient.size(); ++i) {
      result.coefficient[i] = -result.coefficient[i];
    }
    return result;
  }

  Polynomial operator-(const Polynomial & rhs) const { return *this + (-rhs); }
  Polynomial operator*(const NumT & n) const {
    Polynomial result = *this;
    for (size_t i = 0; i < result.coefficient.size(); ++i) {
      result.coefficient[i] *= n;
    }
    return result;
  }

  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    result.coefficient.resize(coefficient.size() + rhs.coefficient.size() - 1, NumT());
    for (size_t i = 0; i < coefficient.size(); ++i) {
      for (size_t j = 0; j < rhs.coefficient.size(); ++j) {
        result.coefficient[i + j] += coefficient[i] * rhs.coefficient[j];
      }
    }
    return result;
  }

  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }
  bool operator==(const Polynomial & rhs) const {
    size_t maxSize = std::max(coefficient.size(), rhs.coefficient.size());
    for (size_t i = 0; i < maxSize; ++i) {
      NumT lhsCoeff = (i < coefficient.size()) ? coefficient[i] : NumT();
      NumT rhsCoeff = (i < rhs.coefficient.size()) ? rhs.coefficient[i] : NumT();
      if (lhsCoeff != rhsCoeff) {
        return false;
      }
    }
    return true;
  }

  void addTerm(const NumT c, unsigned p) {
    if (p >= coefficient.size()) {
      coefficient.resize(p + 1, NumT());
    }
    coefficient[p] += c;
  }
  Polynomial & operator+=(const Polynomial & rhs) {
    *this = *this + rhs;
    return *this;
  }
  Polynomial & operator-=(const Polynomial & rhs) {
    *this = *this - rhs;
    return *this;
  }
  Polynomial & operator*=(const NumT & rhs) {
    *this = *this * rhs;
    return *this;
  }
  Polynomial & operator*=(const Polynomial & rhs) {
    *this = *this * rhs;
    return *this;
  }

  template<typename N>
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p);
};

template<typename NumT>
std::ostream & operator<<(std::ostream & os, const Polynomial<NumT> & p) {
  bool firstTerm = true;
  for (int i = p.coefficient.size() - 1; i >= 0; --i) {
    if (p.coefficient[i] != NumT()) {
      if (!firstTerm)
        os << " + ";
      os << p.coefficient[i] << "*x^" << i;
      firstTerm = false;
    }
  }
  if (firstTerm) {
    os << NumT();
  }
  return os;
}
#endif
