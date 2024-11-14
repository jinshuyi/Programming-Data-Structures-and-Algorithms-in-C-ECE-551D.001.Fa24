#ifndef POLY_HPP
#define POLY_HPP

#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

template<typename NumT>
class Polynomial {
 private:
  std::map<unsigned, NumT> terms;

 public:
  // Default constructor
  Polynomial() { terms[0] = NumT(); }

  // Add a term with coefficient c and exponent p
  void addTerm(const NumT & c, unsigned p) {
    if (terms.count(p) == 0) {
      terms[p] = c;
    }
    else {
      terms[p] += c;
      if (terms[p] == NumT()) {
        terms.erase(p);
      }
    }
  }
  // Negation operator
  Polynomial operator-() const {
    Polynomial result;
    for (typename std::map<unsigned, NumT>::const_iterator it = terms.begin();
         it != terms.end();
         ++it) {
      result.addTerm(-it->second, it->first);
    }
    return result;
  }

  // Addition operator
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (typename std::map<unsigned, NumT>::const_iterator it = rhs.terms.begin();
         it != rhs.terms.end();
         ++it) {
      result.addTerm(it->second, it->first);
    }
    return result;
  }

  // Subtraction operator
  Polynomial operator-(const Polynomial & rhs) const { return *this + (-rhs); }

  // Scalar multiplication operator
  Polynomial operator*(const NumT & n) const {
    Polynomial result;
    for (typename std::map<unsigned, NumT>::const_iterator it = terms.begin();
         it != terms.end();
         ++it) {
      result.addTerm(it->second * n, it->first);
    }
    return result;
  }

  // Polynomial multiplication operator
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    for (typename std::map<unsigned, NumT>::const_iterator it1 = terms.begin();
         it1 != terms.end();
         ++it1) {
      for (typename std::map<unsigned, NumT>::const_iterator it2 = rhs.terms.begin();
           it2 != rhs.terms.end();
           ++it2) {
        result.addTerm(it1->second * it2->second, it1->first + it2->first);
      }
    }
    return result;
  }

  // Addition-assignment operator
  Polynomial & operator+=(const Polynomial & rhs) {
    *this = *this + rhs;
    return *this;
  }

  // Subtraction-assignment operator
  Polynomial & operator-=(const Polynomial & rhs) {
    *this = *this - rhs;
    return *this;
  }

  // Scalar multiplication-assignment operator
  Polynomial & operator*=(const NumT & n) {
    *this = *this * n;
    return *this;
  }

  // Polynomial multiplication-assignment operator
  Polynomial & operator*=(const Polynomial & rhs) {
    *this = *this * rhs;
    return *this;
  }

  bool operator==(const Polynomial & rhs) const {
    // 清除零系数项，确保比较正确
    const_cast<Polynomial *>(this)->cleanUp();
    const_cast<Polynomial *>(&rhs)->cleanUp();
    return terms == rhs.terms;
  }

  // Inequality
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }

  // Friend function for outputting polynomial
  template<typename N>
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p);

 private:
  // Helper function to remove zero terms
  void cleanUp() {
    for (typename std::map<unsigned, NumT>::iterator it = terms.begin();
         it != terms.end();) {
      if (it->second == NumT()) {
        it = terms.erase(it);
      }
      else {
        ++it;
      }
    }
  }
};

// Output operator for Polynomial
template<typename NumT>
std::ostream & operator<<(std::ostream & os, const Polynomial<NumT> & p) {
  bool first = true;
  for (typename std::map<unsigned, NumT>::const_reverse_iterator it = p.terms.rbegin();
       it != p.terms.rend();
       ++it) {
    if (it->second == NumT())
      continue;
    if (!first)
      os << " + ";
    first = false;
    os << it->second << "*x^" << it->first;
  }
  if (first)
    os << NumT();  // output "0" if polynomial is zero
  return os;
}

#endif
