#ifndef POLY_HPP
#define POLY_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

template<typename NumT>
class Polynomial {
 private:
  std::map<int, NumT> terms;  // 存储多项式的项，键为指数，值为系数

  // 辅助函数：清除系数为零的项
  void cleanUp() {
    for (typename std::map<int, NumT>::iterator it = terms.begin(); it != terms.end();) {
      if (it->second == NumT(0)) {
        it = terms.erase(it);  // 移除系数为零的项
      }
      else {
        ++it;
      }
    }
  }

 public:
  // 默认构造函数
  Polynomial() {}

  // 添加项到多项式中
  void addTerm(int exponent, NumT coefficient) {
    terms[exponent] += coefficient;
    cleanUp();  // 确保清除零系数项
  }

  // 运算符重载：加法
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (typename std::map<int, NumT>::const_iterator it = rhs.terms.begin();
         it != rhs.terms.end();
         ++it) {
      result.terms[it->first] += it->second;
    }
    result.cleanUp();
    return result;
  }

  // 运算符重载：减法
  Polynomial operator-(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (typename std::map<int, NumT>::const_iterator it = rhs.terms.begin();
         it != rhs.terms.end();
         ++it) {
      result.terms[it->first] -= it->second;
    }
    result.cleanUp();
    return result;
  }

  // 运算符重载：乘法
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    for (typename std::map<int, NumT>::const_iterator it1 = terms.begin();
         it1 != terms.end();
         ++it1) {
      for (typename std::map<int, NumT>::const_iterator it2 = rhs.terms.begin();
           it2 != rhs.terms.end();
           ++it2) {
        int newExponent = it1->first + it2->first;
        NumT newCoefficient = it1->second * it2->second;
        result.terms[newExponent] += newCoefficient;
      }
    }
    result.cleanUp();
    return result;
  }

  // 赋值运算符重载
  Polynomial & operator=(const Polynomial & rhs) {
    if (this != &rhs) {
      terms = rhs.terms;
    }
    return *this;
  }

  // 比较运算符重载：等于
  bool operator==(const Polynomial & rhs) const {
    Polynomial lhsCopy = *this;
    Polynomial rhsCopy = rhs;
    lhsCopy.cleanUp();
    rhsCopy.cleanUp();
    return lhsCopy.terms == rhsCopy.terms;
  }

  // 比较运算符重载：不等于
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }

  // 友元函数：流输出运算符重载
  friend std::ostream & operator<<(std::ostream & os, const Polynomial & poly) {
    bool first = true;
    for (typename std::map<int, NumT>::const_reverse_iterator it = poly.terms.rbegin();
         it != poly.terms.rend();
         ++it) {
      if (first) {
        first = false;
      }
      else {
        os << (it->second >= NumT(0) ? " + " : " - ");
      }
      if (it->second < NumT(0)) {
        os << -it->second;
      }
      else {
        os << it->second;
      }
      if (it->first > 0) {
        os << "x";
        if (it->first > 1) {
          os << "^" << it->first;
        }
      }
    }
    if (first) {
      os << "0";
    }
    return os;
  }
};

#endif  // POLY_HPP
