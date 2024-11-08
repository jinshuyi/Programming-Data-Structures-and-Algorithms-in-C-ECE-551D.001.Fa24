#include <iostream>
#include <vector>

template<typename NumT>
class Polynomial {
 private:
  std::vector<NumT> coefficients;  // coefficients[i] is the coefficient for x^i

 public:
  // 1. Default Constructor: Initialize to "0"
  Polynomial() : coefficients(1, NumT()) {}

  // 2. Add another Polynomial
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result;
    size_t maxDegree = std::max(coefficients.size(), rhs.coefficients.size());
    result.coefficients.resize(maxDegree, NumT());

    for (size_t i = 0; i < maxDegree; ++i) {
      if (i < coefficients.size())
        result.coefficients[i] += coefficients[i];
      if (i < rhs.coefficients.size())
        result.coefficients[i] += rhs.coefficients[i];
    }
    return result;
  }

  // 3. Negation
  Polynomial operator-() const {
    Polynomial result;
    result.coefficients.resize(coefficients.size());
    for (size_t i = 0; i < coefficients.size(); ++i) {
      result.coefficients[i] = -coefficients[i];
    }
    return result;
  }

  // 4. Subtraction
  Polynomial operator-(const Polynomial & rhs) const { return *this + (-rhs); }

  // 5. Scalar multiplication
  Polynomial operator*(const NumT & n) const {
    Polynomial result;
    result.coefficients.resize(coefficients.size());
    for (size_t i = 0; i < coefficients.size(); ++i) {
      result.coefficients[i] = coefficients[i] * n;
    }
    return result;
  }

  // 6. Polynomial multiplication
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    result.coefficients.resize(coefficients.size() + rhs.coefficients.size() - 1, NumT());
    for (size_t i = 0; i < coefficients.size(); ++i) {
      for (size_t j = 0; j < rhs.coefficients.size(); ++j) {
        result.coefficients[i + j] += coefficients[i] * rhs.coefficients[j];
      }
    }
    return result;
  }

  // 7. Inequality and equality operators
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }
  bool operator==(const Polynomial & rhs) const {
    size_t maxDegree = std::max(coefficients.size(), rhs.coefficients.size());
    for (size_t i = 0; i < maxDegree; ++i) {
      NumT thisCoeff = i < coefficients.size() ? coefficients[i] : NumT();
      NumT rhsCoeff = i < rhs.coefficients.size() ? rhs.coefficients[i] : NumT();
      if (thisCoeff != rhsCoeff)
        return false;
    }
    return true;
  }

  // 8. Adding a term to the Polynomial
  void addTerm(const NumT & c, unsigned p) {
    if (p >= coefficients.size())
      coefficients.resize(p + 1, NumT());
    coefficients[p] += c;
  }

  // Compound operators
  Polynomial & operator+=(const Polynomial & rhs) {
    *this = *this + rhs;
    return *this;
  }

  Polynomial & operator-=(const Polynomial & rhs) {
    *this = *this - rhs;
    return *this;
  }

  Polynomial & operator*=(const Polynomial & rhs) {
    *this = *this * rhs;
    return *this;
  }

  Polynomial & operator*=(const NumT & rhs) {
    *this = *this * rhs;
    return *this;
  }

  // Friend output operator
  template<typename N>
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p) {
    bool isFirstTerm = true;
    for (size_t i = p.coefficients.size(); i-- > 0;) {
      if (p.coefficients[i] != NumT()) {
        if (!isFirstTerm)
          os << " + ";
        os << p.coefficients[i] << "*x^" << i;
        isFirstTerm = false;
      }
    }
    if (isFirstTerm)
      os << NumT();  // If Polynomial is 0
    return os;
  }
};
