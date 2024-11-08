#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>  // 确保包含 vector

template<typename NumT>
class Polynomial {
 private:
  std::map<unsigned, NumT> terms;

 public:
  Polynomial() { terms[0] = NumT(); }

  Polynomial(const Polynomial & other) : terms(other.terms) {}

  Polynomial & operator=(const Polynomial & rhs) {
    if (this != &rhs) {
      terms = rhs.terms;
    }
    return *this;
  }

  // 加法运算符重载
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result(*this);
    typename std::map<unsigned, NumT>::const_iterator it;
    for (it = rhs.terms.begin(); it != rhs.terms.end(); ++it) {
      result.addTerm(it->second, it->first);
    }
    return result;
  }

  // += 复合赋值运算符重载
  Polynomial & operator+=(const Polynomial & rhs) {
    typename std::map<unsigned, NumT>::const_iterator it;
    for (it = rhs.terms.begin(); it != rhs.terms.end(); ++it) {
      this->addTerm(it->second, it->first);
    }
    return *this;
  }

  // -= 复合赋值运算符重载
  Polynomial & operator-=(const Polynomial & rhs) {
    *this += -rhs;
    return *this;
  }

  // 标量 *= 运算符重载
  Polynomial & operator*=(const NumT & n) {
    typename std::map<unsigned, NumT>::iterator it;
    for (it = terms.begin(); it != terms.end(); ++it) {
      it->second *= n;
    }
    return *this;
  }

  // 乘以多项式 *= 运算符重载
  Polynomial & operator*=(const Polynomial & rhs) {
    Polynomial result;
    typename std::map<unsigned, NumT>::const_iterator it1, it2;
    for (it1 = terms.begin(); it1 != terms.end(); ++it1) {
      for (it2 = rhs.terms.begin(); it2 != rhs.terms.end(); ++it2) {
        result.addTerm(it1->second * it2->second, it1->first + it2->first);
      }
    }
    *this = result;
    return *this;
  }

  // 加入标量乘法运算符重载
  Polynomial operator*(const NumT & n) const {
    Polynomial result(*this);
    result *= n;
    return result;
  }

  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    typename std::map<unsigned, NumT>::const_iterator it1, it2;
    for (it1 = terms.begin(); it1 != terms.end(); ++it1) {
      for (it2 = rhs.terms.begin(); it2 != rhs.terms.end(); ++it2) {
        result.addTerm(it1->second * it2->second, it1->first + it2->first);
      }
    }
    return result;
  }

  Polynomial operator-() const {
    Polynomial result;
    typename std::map<unsigned, NumT>::const_iterator it;
    for (it = terms.begin(); it != terms.end(); ++it) {
      result.terms[it->first] = -it->second;
    }
    return result;
  }

  Polynomial operator-(const Polynomial & rhs) const { return *this + (-rhs); }

  void addTerm(const NumT & c, unsigned p) {
    if (terms.count(p) > 0) {
      terms[p] += c;
    }
    else {
      terms[p] = c;
    }
    if (terms[p] == NumT()) {
      terms.erase(p);
    }
  }

  friend std::ostream & operator<<(std::ostream & os, const Polynomial & p) {
    bool first = true;
    typename std::map<unsigned, NumT>::const_reverse_iterator it;
    for (it = p.terms.rbegin(); it != p.terms.rend(); ++it) {
      if (!first) {
        os << " + ";
      }
      os << it->second << "*x^" << it->first;
      first = false;
    }
    if (p.terms.empty()) {
      os << "0";
    }
    return os;
  }

  bool operator==(const Polynomial & rhs) const { return terms == rhs.terms; }

  // 重载不等运算符
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }
};
