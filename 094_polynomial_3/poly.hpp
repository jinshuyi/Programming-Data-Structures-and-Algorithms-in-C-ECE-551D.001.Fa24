#ifndef __POLY_H__
#define __POLY_H__
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
template<typename NumT>
class convergence_failure : public std::exception {
 public:
  const NumT value;
  convergence_failure(const NumT & val) : value(val) {}
  virtual const char * what() const throw() { return "Convergence failure"; }
};

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

  NumT eval(const NumT & x) const {
    NumT result = NumT();
    NumT powerOfX = 1;
    for (size_t i = 0; i < coefficient.size(); ++i) {
      result += coefficient[i] * powerOfX;
      powerOfX *= x;
    }
    return result;
  }

  NumT operator()(const NumT & x) const { return eval(x); }
  Polynomial derivative() const {
    Polynomial result;
    if (coefficient.size() <= 1) {
      return result;  // Derivative of constant is 0
    }
    result.coefficient.resize(coefficient.size() - 1);
    for (size_t i = 1; i < coefficient.size(); ++i) {
      result.coefficient[i - 1] = NumT(i) * coefficient[i];
    }
    return result;
  }

  template<typename OtherNumT>
  Polynomial<OtherNumT> convert() const {
    Polynomial<OtherNumT> result;
    result.coefficient.resize(coefficient.size());
    for (size_t i = 0; i < coefficient.size(); ++i) {
      result.coefficient[i] = OtherNumT(coefficient[i]);
    }
    return result;
  }

  template<typename ToleranceT>
  NumT findZero(NumT x,
                unsigned maxSteps,
                const ToleranceT & tolerance,
                const ToleranceT & deriv_tolerance) const {
    Polynomial<NumT> deriv = derivative();

    while (maxSteps > 0) {
      NumT y = eval(x);
      NumT y_prime = deriv.eval(x);

      if (std::abs(y) <= tolerance) {
        return x;
      }
      if (std::abs(y_prime) < deriv_tolerance) {
        throw convergence_failure<NumT>(x);
      }
      x = x - y / y_prime;
      --maxSteps;
    }
    NumT final_y = eval(x);
    if (std::abs(final_y) <= tolerance) {
      return x;
    }
    throw convergence_failure<NumT>(x);
  }

  // Declare Polynomial<anytype> as a friend of Polynomial<NumT>
  template<typename T>
  friend class Polynomial;
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
