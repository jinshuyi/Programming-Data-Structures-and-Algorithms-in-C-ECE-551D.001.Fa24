#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <complex>
#include <iostream>
#include <vector>

template<typename NumT>
class Polynomial {
 private:
  std::vector<std::pair<NumT, unsigned> >
      terms;  // 用来存储多项式的项，每项是一个（系数，指数）对

 public:
  // 默认构造函数，初始化为零多项式
  Polynomial() {}

  // 添加一项到多项式中
  void addTerm(const NumT & coefficient, unsigned power) {
    for (size_t i = 0; i < terms.size(); ++i) {
      if (terms[i].second == power) {
        terms[i].first = terms[i].first + coefficient;
        return;
      }
    }
    terms.push_back(std::make_pair(coefficient, power));
  }

  // 多项式加法
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (size_t i = 0; i < rhs.terms.size(); ++i) {
      result.addTerm(rhs.terms[i].first, rhs.terms[i].second);
    }
    return result;
  }

  // 多项式负号
  Polynomial operator-() const {
    Polynomial result;
    for (size_t i = 0; i < terms.size(); ++i) {
      result.addTerm(-terms[i].first, terms[i].second);
    }
    return result;
  }

  // 多项式减法
  Polynomial operator-(const Polynomial & rhs) const { return *this + (-rhs); }

  // 标量乘法
  Polynomial operator*(const NumT & scalar) const {
    Polynomial result;
    for (size_t i = 0; i < terms.size(); ++i) {
      result.addTerm(terms[i].first * scalar, terms[i].second);
    }
    return result;
  }

  // 多项式乘法
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    for (size_t i = 0; i < terms.size(); ++i) {
      for (size_t j = 0; j < rhs.terms.size(); ++j) {
        NumT new_coefficient = terms[i].first * rhs.terms[j].first;
        unsigned new_power = terms[i].second + rhs.terms[j].second;
        result.addTerm(new_coefficient, new_power);
      }
    }
    return result;
  }

  // 标量乘法赋值
  Polynomial & operator*=(const NumT & scalar) {
    for (size_t i = 0; i < terms.size(); ++i) {
      terms[i].first *= scalar;
    }
    return *this;
  }

  // 多项式乘法赋值
  Polynomial & operator*=(const Polynomial & rhs) {
    Polynomial result;
    for (size_t i = 0; i < terms.size(); ++i) {
      for (size_t j = 0; j < rhs.terms.size(); ++j) {
        NumT new_coefficient = terms[i].first * rhs.terms[j].first;
        unsigned new_power = terms[i].second + rhs.terms[j].second;
        result.addTerm(new_coefficient, new_power);
      }
    }
    terms = result.terms;
    return *this;
  }

  // 比较两个多项式是否相等
  bool operator==(const Polynomial & rhs) const {
    if (terms.size() != rhs.terms.size())
      return false;
    for (size_t i = 0; i < terms.size(); ++i) {
      if (terms[i].first != rhs.terms[i].first || terms[i].second != rhs.terms[i].second)
        return false;
    }
    return true;
  }

  // 比较两个多项式是否不相等
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }

  // 赋值运算符
  Polynomial & operator=(const Polynomial & rhs) {
    if (this != &rhs) {
      terms = rhs.terms;
    }
    return *this;
  }

  // 加法赋值运算符
  Polynomial & operator+=(const Polynomial & rhs) {
    *this = *this + rhs;
    return *this;
  }

  // 减法赋值运算符
  Polynomial & operator-=(const Polynomial & rhs) {
    *this = *this - rhs;
    return *this;
  }

  // 流输出友元函数
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<NumT> & p) {
    if (p.terms.empty()) {
      os << "0";
    }
    else {
      bool first = true;
      for (size_t i = p.terms.size(); i-- > 0;) {
        if (!first)
          os << " + ";
        os << p.terms[i].first << "*x^" << p.terms[i].second;
        first = false;
      }
    }
    return os;
  }
};

#endif  // POLYNOMIAL_HPP
