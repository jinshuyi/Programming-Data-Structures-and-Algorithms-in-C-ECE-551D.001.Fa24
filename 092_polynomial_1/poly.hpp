#ifndef POLY_HPP
#define POLY_HPP

#include <iostream>
#include <sstream>
#include <vector>

template<typename NumT>
class Polynomial {
 public:
  // 存储多项式的每一项（系数，指数对）
  std::vector<std::pair<NumT, int> > terms;

  // 默认构造函数，初始化为空多项式
  Polynomial() {}

  // 构造函数，通过传递项的系数和指数
  Polynomial(const std::vector<std::pair<NumT, int> > & terms) : terms(terms) {}

  // 重载加法操作符
  Polynomial operator+(const Polynomial & other) const {
    Polynomial result;
    size_t i = 0, j = 0;

    while (i < terms.size() && j < other.terms.size()) {
      if (terms[i].second > other.terms[j].second) {
        result.terms.push_back(terms[i++]);
      }
      else if (terms[i].second < other.terms[j].second) {
        result.terms.push_back(other.terms[j++]);
      }
      else {
        result.terms.push_back(
            std::make_pair(terms[i].first + other.terms[j].first, terms[i].second));
        ++i;
        ++j;
      }
    }

    // 将剩余项加入结果中
    while (i < terms.size())
      result.terms.push_back(terms[i++]);
    while (j < other.terms.size())
      result.terms.push_back(other.terms[j++]);

    return result;
  }

  // 重载减法操作符
  Polynomial operator-(const Polynomial & other) const {
    Polynomial result;
    size_t i = 0, j = 0;

    while (i < terms.size() && j < other.terms.size()) {
      if (terms[i].second > other.terms[j].second) {
        result.terms.push_back(terms[i++]);
      }
      else if (terms[i].second < other.terms[j].second) {
        result.terms.push_back(
            std::make_pair(-other.terms[j].first, other.terms[j].second));
        ++j;
      }
      else {
        result.terms.push_back(
            std::make_pair(terms[i].first - other.terms[j].first, terms[i].second));
        ++i;
        ++j;
      }
    }

    // 将剩余项加入结果中
    while (i < terms.size())
      result.terms.push_back(terms[i++]);
    while (j < other.terms.size())
      result.terms.push_back(
          std::make_pair(-other.terms[j].first, other.terms[j].second));

    return result;
  }

  // 重载乘法操作符
  Polynomial operator*(const Polynomial & other) const {
    Polynomial result;
    for (size_t i = 0; i < terms.size(); ++i) {
      for (size_t j = 0; j < other.terms.size(); ++j) {
        NumT coeff = terms[i].first * other.terms[j].first;
        int exp = terms[i].second + other.terms[j].second;
        bool found = false;

        // 查找是否已存在相同指数的项
        for (size_t k = 0; k < result.terms.size(); ++k) {
          if (result.terms[k].second == exp) {
            result.terms[k].first += coeff;
            found = true;
            break;
          }
        }

        // 如果不存在，添加新项
        if (!found) {
          result.terms.push_back(std::make_pair(coeff, exp));
        }
      }
    }

    return result;
  }

  // 重载赋值操作符
  Polynomial & operator=(const Polynomial & other) {
    if (this != &other) {
      terms = other.terms;
    }
    return *this;
  }

  // 重载比较操作符 (==)
  bool operator==(const Polynomial & other) const { return terms == other.terms; }

  // 重载比较操作符 (!=)
  bool operator!=(const Polynomial & other) const { return terms != other.terms; }

  // 添加一项到多项式中
  void addTerm(NumT coeff, int exp) { terms.push_back(std::make_pair(coeff, exp)); }

  // 输出多项式
  friend std::ostream & operator<<(std::ostream & os, const Polynomial & poly) {
    if (poly.terms.empty()) {
      os << "0";
      return os;
    }

    bool firstTerm = true;
    for (size_t i = 0; i < poly.terms.size(); ++i) {
      if (poly.terms[i].first == 0)
        continue;

      if (!firstTerm) {
        os << (poly.terms[i].first > 0 ? " + " : " - ");
      }

      NumT absCoeff =
          poly.terms[i].first < 0 ? -poly.terms[i].first : poly.terms[i].first;
      os << absCoeff;

      if (poly.terms[i].second > 0) {
        os << "*x^" << poly.terms[i].second;
      }
      else if (poly.terms[i].second == 0) {
        os << "*x^0";
      }

      firstTerm = false;
    }

    return os;
  }
};

#endif  // POLY_HPP
