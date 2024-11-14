#ifndef POLY_HPP
#define POLY_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

template<typename NumT>
class Polynomial {
 private:
  std::map<unsigned, NumT> terms;  // 存储每项的指数和系数

 public:
  // 默认构造函数，将多项式初始化为 0
  Polynomial() { terms[0] = NumT(); }

  // 添加项：添加系数为 c、指数为 p 的项
  void addTerm(const NumT & c, unsigned p) {
    terms[p] += c;
    if (terms[p] == NumT()) {
      terms.erase(p);  // 移除系数为 0 的项
    }
  }

  // 重载加法运算符
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (typename std::map<unsigned, NumT>::const_iterator it = rhs.terms.begin();
         it != rhs.terms.end();
         ++it) {
      result.addTerm(it->second, it->first);
    }
    return result;
  }

  // 重载减法运算符
  Polynomial operator-(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (typename std::map<unsigned, NumT>::const_iterator it = rhs.terms.begin();
         it != rhs.terms.end();
         ++it) {
      result.addTerm(-it->second, it->first);
    }
    return result;
  }

  // 重载一元负运算符
  Polynomial operator-() const {
    Polynomial result;
    for (typename std::map<unsigned, NumT>::const_iterator it = terms.begin();
         it != terms.end();
         ++it) {
      result.terms[it->first] = -it->second;
    }
    return result;
  }

  // 重载乘以标量的运算符
  Polynomial operator*(const NumT & n) const {
    Polynomial result;
    for (typename std::map<unsigned, NumT>::const_iterator it = terms.begin();
         it != terms.end();
         ++it) {
      result.terms[it->first] = it->second * n;
    }
    return result;
  }

  // 重载乘法运算符：多项式乘以多项式
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

  // 重载比较运算符==
  bool operator==(const Polynomial & rhs) const { return terms == rhs.terms; }

  // 重载比较运算符!=
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }

  // 重载+=运算符
  Polynomial & operator+=(const Polynomial & rhs) {
    *this = *this + rhs;
    return *this;
  }

  // 重载-=运算符
  Polynomial & operator-=(const Polynomial & rhs) {
    *this = *this - rhs;
    return *this;
  }

  // 重载*=运算符（多项式与标量）
  Polynomial & operator*=(const NumT & rhs) {
    *this = *this * rhs;
    return *this;
  }

  // 重载*=运算符（多项式与多项式）
  Polynomial & operator*=(const Polynomial & rhs) {
    *this = *this * rhs;
    return *this;
  }

  // 输出运算符<<
  friend std::ostream & operator<<(std::ostream & os, const Polynomial & p) {
    if (p.terms.empty()) {
      os << NumT();
      return os;
    }

    bool first = true;
    for (typename std::map<unsigned, NumT>::const_reverse_iterator it = p.terms.rbegin();
         it != p.terms.rend();
         ++it) {
      if (!first) {
        os << " + ";
      }
      first = false;
      os << it->second << "*x^" << it->first;
    }
    return os;
  }
};

#endif
