#ifndef POLY_HPP
#define POLY_HPP

#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <stdexcept>

template<typename NumT>
class Polynomial {
 private:
  std::map<unsigned, NumT> terms;  // 存储多项式的项，键是指数，值是系数

 public:
  // 默认构造函数，初始化为 0
  Polynomial() { terms[0] = NumT(); }

  // 添加项：c * x^p
  void addTerm(const NumT & c, unsigned p) {
    terms[p] += c;
    if (terms[p] == NumT()) {
      terms.erase(p);
    }
  }

  // 加法运算符
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (const auto & term : rhs.terms) {
      result.addTerm(term.second, term.first);
    }
    return result;
  }

  // 负号运算符
  Polynomial operator-() const {
    Polynomial result;
    for (const auto & term : terms) {
      result.terms[term.first] = -term.second;
    }
    return result;
  }

  // 减法运算符
  Polynomial operator-(const Polynomial & rhs) const { return *this + (-rhs); }

  // 乘以常数
  Polynomial operator*(const NumT & n) const {
    Polynomial result;
    for (const auto & term : terms) {
      result.terms[term.first] = term.second * n;
    }
    return result;
  }

  // 多项式相乘
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    for (const auto & term1 : terms) {
      for (const auto & term2 : rhs.terms) {
        result.addTerm(term1.second * term2.second, term1.first + term2.first);
      }
    }
    return result;
  }

  // 判断相等
  bool operator==(const Polynomial & rhs) const { return terms == rhs.terms; }

  // 判断不等
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }

  // 复合赋值操作符
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

  // 输出运算符
  template<typename T>
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<T> & p);
};

// 输出多项式到输出流
template<typename T>
std::ostream & operator<<(std::ostream & os, const Polynomial<T> & p) {
  bool first = true;
  for (auto it = p.terms.rbegin(); it != p.terms.rend(); ++it) {
    if (!first) {
      os << " + ";
    }
    os << it->second << "*x^" << it->first;
    first = false;
  }
  if (first) {
    os << T();
  }
  return os;
}

#endif  // POLY_HPP
